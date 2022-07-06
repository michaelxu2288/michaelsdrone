#include <drone.h>
#include <pca9685.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <mpu6050.h>
#include <logger.h>
#include <config.h>
#include <socket.h>
#include <bmp390.h>

// #include <cmath>
#include <limits>
#include <math.h>
#include <filter.h>
#include <pid.h>
#include <cmath>
#include <mutex>
// #define nan std::numeric_limits<double>::quiet_NaN()
#define G 9.81

// static double nan = std::numeric_limits<double>::quiet_NaN();

static double mpu6050_data[6];
static double filtered_mpu6050_data[6];
static filter::filter mpu6050_filters[6];
static math::quarternion orientation;

static math::vector orientation_euler;
static math::vector position(0, 0, 0), velocity(0, 0, 0);

static double bmp390_data[3];


static double throttle = 0.0;
static double motor_fl_spd, motor_fr_spd, motor_bl_spd, motor_br_spd;

static bool alive = true;

static int sensor_ref_rate, sensor_sleep_int;
static int upper_sensor_freq_cutoff;
static double lower_sensor_freq_cutoff;
static std::thread sensor_thread;
static int settle_length;
static double sensor_tau;
static double sensor_tolerance, sensor_tolerance_sqrd;


static int message_thread_ref_rate, message_sleep_int;
static std::thread message_thread;
static std::string socket_path;

static bool zero_flag = false;
static bool calib_flag = false;

static pid /* x_controller, y_controller, */ z_controller;
static pid roll_controller, pitch_controller, yaw_controller;

static double debug_vals[6] = {0, 0, 0, 0, 0, 0};


pid * drone::get_roll_controller(){
    return &roll_controller;
}

pid * drone::get_pitch_controller(){
    return &pitch_controller;
}

pid * drone::get_z_controller(){
    return &z_controller;
}

void clear_led(){
    pca9685::set_pwm_ms(LED_RUN_PIN, 0);
    pca9685::set_pwm_ms(LED_AUTO_PIN, 0);
    pca9685::set_pwm_ms(LED_ERROR_PIN, 0);
    pca9685::set_pwm_ms(LED_WHITE_PIN, 0);
    pca9685::set_pwm_ms(LED_BLUE_PIN, 0);
}
void drone::force_terminate(){
    clear_led();
    pca9685::set_pwm_ms(LED_RUN_PIN, 0);
    pca9685::set_pwm_ms(LED_ERROR_PIN, PWM_FULL-1);
    drone::set_all(0);
}


void terminate_handle(int signum){
    drone::force_terminate();
    exit(0);
}


void drone::init(){
    // Setup pca9685
    pca9685::set_frequency(50);
    pca9685::init();
    pca9685::wake_up();
    
    // Attach handle for ctrl+c
    signal(SIGINT, terminate_handle);
    atexit(drone::force_terminate);

    //initial LED control
    clear_led();
    pca9685::set_pwm_ms(LED_RUN_PIN, PWM_FULL - 1);
}

void drone::arm(){

}

void drone::load_configuration(){
    config::load_file();
    
    sensor_ref_rate = config::get_config_int("sensor_ref_rate", 60);
    upper_sensor_freq_cutoff = config::get_config_int("upper_sensor_freq_cutoff", 5);
    lower_sensor_freq_cutoff = config::get_config_dbl("lower_sensor_freq_cutoff", 0.01);
    settle_length = config::get_config_int("settle_length", 200);
    sensor_tau = config::get_config_dbl("sensor_tau", 0.02);
    sensor_tolerance = config::get_config_dbl("sensor_tolerance", 0.1);

    message_thread_ref_rate = config::get_config_int("message_ref_rate", 10);
    socket_path = config::get_config_str("socket_path", "./run/drone");

    config::write_to_file();
    
    sensor_sleep_int = 1000000 / sensor_ref_rate;
    message_sleep_int = 1000000 / message_thread_ref_rate;
    sensor_tolerance_sqrd = sensor_tolerance * sensor_tolerance;

    for(int i = 0; i < 3; i ++){
        // mpu6050_filters[i] = filter::high_pass(sensor_ref_rate, lower_sensor_freq_cutoff);
        mpu6050_filters[i] = filter::low_pass(sensor_ref_rate, upper_sensor_freq_cutoff);
        // mpu6050_filters[i] = filter::none();
    }
    for(int i = 3; i < 6; i ++){
        mpu6050_filters[i] = filter::none();
    }

}

void drone::set_all(double per){
    motor_bl_spd = motor_br_spd = motor_fl_spd = motor_fr_spd = per;
    #ifdef ENABLE_MOTOR
        int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
        pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
    #endif
}

void drone::set_diagonals(short diagonal, double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    switch (diagonal)
    {
    case FLBR_DIAGONAL:
        motor_br_spd = motor_fl_spd = per;
        #ifdef ENABLE_MOTOR
            pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
            pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
        #endif
        break;
    case FRBL_DIAGONAL:
        motor_bl_spd = motor_fr_spd = per;
        #ifdef ENABLE_MOTOR
            pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
            pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        #endif
        break;
    
    default:
        break;
    }
}

void drone::set_motor(short motor, double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    switch(motor){
    case MOTOR_FL:
        motor_fl_spd = per;
        #ifdef ENABLE_MOTOR
            pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
        #endif
        break;
    case MOTOR_FR:
        motor_fr_spd = per;
        #ifdef ENABLE_MOTOR
            pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
        #endif
        break;
    case MOTOR_BL:
        motor_bl_spd = per;
        #ifdef ENABLE_MOTOR
            pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        #endif
        break;
    case MOTOR_BR:
        motor_br_spd = per;
        #ifdef ENABLE_MOTOR
            pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
        #endif
        break;
    default:break;
    }
}



void drone::set_throttle(double per){
    throttle = per;
    // drone::set_all(throttle);
}

void drone::destroy(){
    
    set_all(0);

    pca9685::destroy();
}


int next_token(const std::string & tokenized, int i, std::string & out){
    int k = tokenized.find(" ", i);
    if(k < 0){
        out = tokenized.substr(i);
        return tokenized.length();
    }
    out = tokenized.substr(i, k);
    return k + 1;
}


void drone::run_command(const std::string& s){
    std::string k;
    run_command(s, k);
}

void drone::run_command(const std::string& s, std::string& msg){
    std::string command;
    int i = next_token(s, 0, command);
    // string v;
    // i = next_token(m, i, v);
    // float value = atof(v.c_str());
    int len = s.length();

    if(command == "thrtl"){
        i = next_token(s, i, command);
        if(command == ""){
            msg = "Throttle is "+std::to_string(throttle)+"% power.";
        }else {
            float value = atof(command.c_str());
            std::cout << "Throttle Argument: " << command << "\n";
            set_throttle(value);
            msg = "Throttle set to " + std::to_string(value) + "% power.";
        }
    }else if(command == ""){

    }else {
        msg = "I don't know that command! :(";
        return;
    }
}



static std::mutex sensor_thread_mutex, message_thread_mutex;
static std::thread rel_config;

void settle(){
    for(int i = 0; i < settle_length; i++){
        mpu6050::read(mpu6050_data);

        for(int i = 0; i < 6; i ++){
            filtered_mpu6050_data[i] = mpu6050_filters[i][mpu6050_data[i]];
        }

        bmp390::get_data(bmp390_data);
        usleep(sensor_sleep_int);
    }
}

void sensor_thread_funct(){
    logger::info("Sensor thread alive!");

    orientation = math::quarternion(1, 0, 0, 0);

    math::quarternion euler_q;
    math::vector euler_v;
    math::vector temp;
    auto then = std::chrono::steady_clock::now();
    auto start = then;
    auto now = std::chrono::steady_clock::now();

    logger::info("Settling sensors");

    settle();

    logger::info("Settled sensor filters");
    while(alive){
        std::lock_guard<std::mutex> sensor_lock_guard(sensor_thread_mutex);
        now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::nanoseconds> (now - then).count() * 0.000000001;
        int t_since = std::chrono::duration_cast<std::chrono::nanoseconds> (now - start).count();
        then = now;
        
        { // MPU6050 Sensor Read, Filter & Dead Reckoning
            mpu6050::read(mpu6050_data);
            mpu6050_data[4] *= -1;
            mpu6050_data[5] *= -1;
            for(int i = 0; i < 6; i ++){
                filtered_mpu6050_data[i] = mpu6050_filters[i][mpu6050_data[i]];
            }
                    
            euler_v = math::vector(filtered_mpu6050_data[3]*dt*DEG_TO_RAD, filtered_mpu6050_data[4]*dt*DEG_TO_RAD, filtered_mpu6050_data[5]*dt*DEG_TO_RAD);
            euler_q = math::quarternion::fromEulerZYX(euler_v);
            orientation = euler_q*orientation;
            orientation_euler = math::quarternion::toEuler(orientation);


            double a_dist_from_one_sqrd = filtered_mpu6050_data[0] * filtered_mpu6050_data[0] + filtered_mpu6050_data[1] * filtered_mpu6050_data[1] + filtered_mpu6050_data[2] * filtered_mpu6050_data[2] - 1;
            if((a_dist_from_one_sqrd < 0 ? a_dist_from_one_sqrd > - sensor_tolerance_sqrd : a_dist_from_one_sqrd < sensor_tolerance_sqrd)){
                double roll = atan2(filtered_mpu6050_data[1], filtered_mpu6050_data[2]);
                double pitch = atan2((filtered_mpu6050_data[0]) , sqrt(filtered_mpu6050_data[1] * filtered_mpu6050_data[1] + filtered_mpu6050_data[2] * filtered_mpu6050_data[2]));

                orientation_euler.x = orientation_euler.x * (1 - sensor_tau) + roll * sensor_tau;
                orientation_euler.y = orientation_euler.y * (1 - sensor_tau) + pitch * sensor_tau;
                
                orientation = math::quarternion::fromEulerZYX(orientation_euler);
                orientation_euler = math::quarternion::toEuler(orientation);
            }

            temp = velocity * dt;
            position = position + temp;
            temp = math::vector(filtered_mpu6050_data[0]*dt*G, filtered_mpu6050_data[1]*dt*G, -filtered_mpu6050_data[2]*dt*G);
            // logger::info("{:.2f} {:.2f} {:.2f}", temp.x, temp.y, temp.z);
            temp = math::quarternion::rotateVector(orientation, temp);
            temp.z += G*dt;
            velocity = velocity + temp;
        }

        { // BMP390 Sensor Read
            bmp390::get_data(bmp390_data);
        }

        if(zero_flag){
            orientation = math::quarternion(1, 0, 0, 0);

            velocity = math::vector(0, 0, 0);
            position = math::vector(0, 0, 0);

            logger::info("Zeroed");
            zero_flag = false;
        }

        if(calib_flag){
            orientation = math::quarternion(1, 0, 0, 0);

            velocity = math::vector(0, 0, 0);
            position = math::vector(0, 0, 0);

            logger::info("Calibrated");
            mpu6050::calibrate(7);
            settle();

            calib_flag = false;
            
        }
        
        usleep(sensor_sleep_int);
    }
}

void reload_config_thread(){
    std::lock_guard<std::mutex> sensor_lock_guard(sensor_thread_mutex);
    std::lock_guard<std::mutex> message_lock_guard(message_thread_mutex);

    logger::info("Acquired locks!");
    drone::load_configuration();
    logger::info("Releasing locks!");
}

void message_thread_funct(){
    logger::info("Message thread alive!");

    sock::socket client(sock::unix, sock::tcp);
    sock::un_connection unix_connection = client.un_connect(socket_path.c_str());
    char send[1024];
    char recv[1024];
    while(alive){
        std::lock_guard<std::mutex> message_lock_guard(message_thread_mutex);
        

        // | zero | calibrate | Reload Config |
        // |  0   |     1     |       2       |
        if(unix_connection.can_read()){
            logger::info("YOO DATA!");
            
            int len = unix_connection.read(recv, 50);
            int cmd = atoi(recv);

            switch(cmd){
            case 0:
                logger::info("Zeroing");
                zero_flag = true;
                break;
            case 1:
                logger::info("Calibrating");
                calib_flag = true;
                break;
            case 2:
                logger::info("Reloading configuration");
                rel_config = std::thread(reload_config_thread);
                break;
            default:
                logger::warn("Unknown cmd \"{}\"", cmd);
            }

            // logger::info("Message: \"{}\"", recv);
        }

        // |                  Setpoints                    |                      Error                    |     Motor Speed   |
        // | x | y | z | vx | vy | vz | roll | pitch | yaw | x | y | z | vx | vy | vz | roll | pitch | yaw | fl | fr | bl | br |
        // | 0 | 1 | 2 | 3  | 4  | 5  |  6   |   7   |  8  | 9 |10 |11 | 12 | 13 | 14 |  15  |  16   | 17  | 18 | 19 | 20 | 21 |


        // |                MPU6050                  |                 Dead Reckoned                 |             BMP390                |
        // | Ax | Ay | Az | ARroll | ARpitch | ARyaw | Vx | Vy | Vz | X | Y | Z | Roll | Pitch | Yaw | Temperature | Pressure | Altitude |
        // | 0  | 1  | 2  |   3    |    4    |   5   | 6  | 7  | 8  | 9 |10 |11 |  12  |  13   | 14  |     15      |    16    |    17    |
        
        // |                  Setpoints                    |                      Error                    |     Motor Speed   |         Debug         |
        // | x | y | z | vx | vy | vz | roll | pitch | yaw | x | y | z | vx | vy | vz | roll | pitch | yaw | fl | fr | bl | br | 0 | 1 | 2 | 3 | 4 | 5 |
        // |18 |19 |20 | 21 | 22 | 23 |  24  |  25   | 26  |27 |28 |29 | 30 | 31 | 32 |  33  |  34   | 35  | 36 | 37 | 38 | 39 |40 |41 |42 |43 |44 |45 |

        sprintf(send, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
            filtered_mpu6050_data[0]*G, filtered_mpu6050_data[1]*G, (filtered_mpu6050_data[2])*G, filtered_mpu6050_data[3]*DEG_TO_RAD, filtered_mpu6050_data[4]*DEG_TO_RAD, filtered_mpu6050_data[5]*DEG_TO_RAD,
            velocity.x, velocity.y, velocity.z, position.x, position.y, position.z, orientation_euler.x, orientation_euler.y, orientation_euler.z,
            bmp390_data[0], bmp390_data[1], bmp390_data[2], // 18
            nan, nan, z_controller.setpoint, nan, nan, nan, roll_controller.setpoint, pitch_controller.setpoint, yaw_controller.setpoint,
            nan, nan, z_controller.old_error, nan, nan, nan, roll_controller.old_error, pitch_controller.old_error, yaw_controller.old_error,
            motor_fl_spd, motor_fr_spd, motor_bl_spd, motor_br_spd,
            debug_vals[0], debug_vals[1], debug_vals[2], debug_vals[3], debug_vals[4], debug_vals[5]
            );
        unix_connection.send(send, strlen(send));
        // logger::info("{:.2f} {:.2f}", debug_vals[0], debug_vals[1]);


        usleep(message_sleep_int);
    }
}






void drone::init_sensors(bool thread) {
    logger::info("Initializing MPU6050.");
    mpu6050::init();
    mpu6050::set_accl_set(mpu6050::accl_range::g_2);
    mpu6050::set_gyro_set(mpu6050::gyro_range::deg_250);
    mpu6050::set_clk(mpu6050::clk::y_gyro);
    mpu6050::set_fsync(mpu6050::fsync::input_dis);
    mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
    mpu6050::wake_up();

    mpu6050::calibrate(7);

    logger::info("Finished intializing the MPU6050.");

    logger::info("Initializing BMP390.");
    bmp390::init();
    logger:info("Finished initializing the BMP390.");

    if(thread){
        logger::info("Starting up sensor thread.");
        sensor_thread = std::thread(sensor_thread_funct);
    }
}

void drone::init_messsage_thread(bool thread){
    if(thread){
        logger::info("Starting up message thread.");
        logger::info("Message rate: {}", message_thread_ref_rate);
        message_thread = std::thread(message_thread_funct);
    }
}



void drone::destroy_message_thread(){
    logger::info("Joining message thread.");
    alive = false;
    message_thread.join();
    logger::info("Joined message thread.");
}

void drone::destroy_sensors(){
    logger::info("Joining sensor thread.");
    alive = false;
    sensor_thread.join();
    logger::info("Joined sensor thread.");
}

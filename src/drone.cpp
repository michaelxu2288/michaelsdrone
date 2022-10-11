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
#include <parameters.h>

#include <gamepad.h>
// #include <cmath>
#include <limits>
#include <math.h>
#include <filter.h>
#include <pid.h>
#include <cmath>
#include <mutex>

#include <timer.h>

// #define nan std::numeric_limits<double>::quiet_NaN()
#define G 9.81

// static double nan = std::numeric_limits<double>::quiet_NaN();

static double mpu6050_data[6];
static double filtered_mpu6050_data[6];
static filter::filter mpu6050_filters[6];
static math::quarternion orientation;

// static double dt;

static math::vector orientation_euler;
static math::vector position(0, 0, 0), velocity(0, 0, 0);

static double bmp390_data[3];
static double valt;
static double old_altitude;
static double initial_altitude;


static double throttle = 0.0;
static double motor_fl_spd, motor_fr_spd, motor_bl_spd, motor_br_spd;

static bool alive = true;

static int sensor_ref_rate, sensor_sleep_int;
static int upper_sensor_freq_cutoff;
static double lower_sensor_freq_cutoff;
static std::thread sensor_thread;
static int settle_length;
static double sensor_roll_pitch_tau;
static double sensor_g_tolerance, sensor_g_tolerance_sqrd;

static double upper_pressure_freq_cutoff, upper_vz_freq_cutoff;
static filter::filter pressure_filter;
static filter::filter vzfilter;
static double sensor_z_tau;

static int message_thread_ref_rate, message_sleep_int;
static std::thread message_thread;
static std::string socket_path;

static bool zero_flag = false;
static bool calib_flag = false;

static double trim, front_multiplier = 1;
static pid /* x_controller, y_controller, */ z_controller;
static pid roll_controller, pitch_controller, vyaw_controller;
static double thrust = 0;
static double yawthrust = 0;

static double cpu_usg=-1, battery=-1;
static bool cntrller_connected = false;

static double debug_vals[6] = {0, 0, 0, 0, 0, 0};

static timer sensor_timer;

enum state {
    configuring = 0, ready = 1, calibrating = 2, idle = 3, init = 4, settling = 5, destroying = 6
};

static state curr_state = state::init;


void drone::set_ctrller_connected_flag(bool connected) {
    cntrller_connected = connected;
}

pid * drone::get_roll_controller(){
    return &roll_controller;
}

pid * drone::get_pitch_controller(){
    return &pitch_controller;
}

pid * drone::get_z_controller(){
    return &z_controller;
}

pid * drone::get_vyaw_controller(){
    return &vyaw_controller;
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
    logger::info("Initializing drone.");
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

void setup_filters(){
    for(int i = 0; i < 3; i ++){
        mpu6050_filters[i] = filter::low_pass(sensor_ref_rate, upper_sensor_freq_cutoff);
    }
    for(int i = 3; i < 6; i ++){
        mpu6050_filters[i] = filter::none();
    }

    mpu6050_filters[5] = filter::low_pass(sensor_ref_rate, upper_sensor_freq_cutoff);

    pressure_filter = filter::low_pass(sensor_ref_rate, upper_pressure_freq_cutoff);
    vzfilter = filter::low_pass(sensor_ref_rate, upper_vz_freq_cutoff);

    z_controller.dFilter = filter::low_pass(sensor_ref_rate, 1);
    roll_controller.dFilter = filter::low_pass(sensor_ref_rate, 1);
    pitch_controller.dFilter = filter::low_pass(sensor_ref_rate, 1);
    vyaw_controller.dFilter = filter::low_pass(sensor_ref_rate, 1);
}


void load_pid_config(){
    config::load_file();
    
    z_controller.kP = config::get_config_dbl("pid.z.kP", 0.01);
    z_controller.kI = config::get_config_dbl("pid.z.kI", 0.00);
    z_controller.kD = config::get_config_dbl("pid.z.kD", 0.00);
    
    roll_controller.kP = config::get_config_dbl("pid.r.kP", 0.01);
    roll_controller.kI = config::get_config_dbl("pid.r.kI", 0.00);
    roll_controller.kD = config::get_config_dbl("pid.r.kD", 0.00);
    
    pitch_controller.kP = config::get_config_dbl("pid.p.kP", 0.01);
    pitch_controller.kI = config::get_config_dbl("pid.p.kI", 0.00);
    pitch_controller.kD = config::get_config_dbl("pid.p.kD", 0.00);
    
    vyaw_controller.kP = config::get_config_dbl("pid.vyaw.kP", 0.01);
    vyaw_controller.kI = config::get_config_dbl("pid.vyaw.kI", 0.00);
    vyaw_controller.kD = config::get_config_dbl("pid.vyaw.kD", 0.00);
    
    trim = config::get_config_dbl("pid.trim", 0.0);

    config::write_to_file();

    logger::lconfig("pid.z.kP: {}", z_controller.kP);
    logger::lconfig("pid.z.kI: {}", z_controller.kI);
    logger::lconfig("pid.z.kD: {}", z_controller.kD);

    logger::lconfig("pid.r.kP: {}", roll_controller.kP);
    logger::lconfig("pid.r.kI: {}", roll_controller.kI);
    logger::lconfig("pid.r.kD: {}", roll_controller.kD);

    logger::lconfig("pid.p.kP: {}", pitch_controller.kP);
    logger::lconfig("pid.p.kI: {}", pitch_controller.kI);
    logger::lconfig("pid.p.kD: {}", pitch_controller.kD);

    logger::lconfig("pid.vy.kP: {}", vyaw_controller.kP);
    logger::lconfig("pid.vy.kI: {}", vyaw_controller.kI);
    logger::lconfig("pid.vy.kD: {}", vyaw_controller.kD);

    logger::lconfig("pid.trim: {}", trim);
}

void drone::load_configuration(){
    config::load_file();
    
    sensor_ref_rate = config::get_config_int("sensor.ref_rate", 60);
    upper_sensor_freq_cutoff = config::get_config_dbl("sensor.mpu6050.upper_freq_cutoff", 5);
    lower_sensor_freq_cutoff = config::get_config_dbl("sensor.mpu6050.lower_freq_cutoff", 0.01);
    settle_length = config::get_config_int("sensor.settle_length", 200);
    sensor_roll_pitch_tau = config::get_config_dbl("sensor.roll_pitch_tau", 0.02);
    sensor_g_tolerance = config::get_config_dbl("sensor.g_tolerance", 0.1);

    message_thread_ref_rate = config::get_config_int("message.ref_rate", 10);
    socket_path = config::get_config_str("message.socket_path", "./run/drone");

    upper_vz_freq_cutoff = config::get_config_dbl("sensor.vertical_v.upper_freq_cutoff", 2);
    upper_pressure_freq_cutoff = config::get_config_dbl("sensor.pressure.upper_freq_cutoff", 5);
    sensor_z_tau = config::get_config_dbl("sensor.z_tau", 0.02);



    config::write_to_file();

    load_pid_config();
    
    sensor_sleep_int = 1000000 / sensor_ref_rate;
    message_sleep_int = 1000000 / message_thread_ref_rate;
    sensor_g_tolerance_sqrd = sensor_g_tolerance * sensor_g_tolerance;

    logger::lconfig("Settle Length: {}", settle_length);
    logger::lconfig("Sensor Refresh Rate: {}hz", sensor_ref_rate);
    logger::lconfig("Message Refresh Rate: {}hz", message_thread_ref_rate);
    logger::lconfig("Socket Location: {}", socket_path);
    logger::lconfig("Sensor G Tolerance: {}", sensor_g_tolerance);
    logger::lconfig("Sensor Roll Pitch Tau: {}", sensor_roll_pitch_tau);
    logger::lconfig("Sensor Z Tau: {}", sensor_z_tau);
    logger::lconfig("Upper Accelerometer Frequency Cutoff: {}", upper_sensor_freq_cutoff);
    logger::lconfig("Upper Pressure Frequency Cutoff: {}", upper_pressure_freq_cutoff);
    logger::lconfig("Upper Vz Frequency Cutoff: {}", upper_vz_freq_cutoff);

    setup_filters();

    sensor_timer.interval = 1000 / sensor_ref_rate;
}

void drone::enable(bool fl, bool fr, bool bl, bool br){

}

void drone::set_all(double per){
    if(per < 0){
        per = 0;
    }else if(per > 1){
        per = 1;
    }
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
    if(per < 0){
        per = 0;
    }else if(per > 1){
        per = 1;
    }
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
    if(per < 0){
        per = 0;
    }else if(per > 1){
        per = 1;
    }
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
    logger::info("Settling sensors");
    state old = curr_state;
    curr_state = state::settling;
    for(int i = 0; i < settle_length; i++){
        mpu6050::read(mpu6050_data);

        for(int i = 0; i < 6; i ++){
            filtered_mpu6050_data[i] = mpu6050_filters[i][mpu6050_data[i]];
        }

        bmp390::get_data(bmp390_data);
        bmp390_data[1] = pressure_filter[bmp390_data[1]];
        bmp390_data[2] = bmp390::get_height(bmp390_data[0], bmp390_data[1]);

        old_altitude = bmp390_data[2];
        initial_altitude = bmp390_data[2];
        usleep(sensor_sleep_int);
    }

    // logger::info("Initial altitude: {:.2f} m", 3.281 * initial_altitude);
    logger::info("Settled sensors");
    curr_state = old;
}

void calibrate(){
    state old = curr_state;
    logger::info("Calibrating sensors");
    logger::info("Calibrating MPU6050");
    curr_state = state::calibrating;
    orientation = math::quarternion(1, 0, 0, 0);

    velocity = math::vector(0, 0, 0);
    position = math::vector(0, 0, 0);

    mpu6050::calibrate(7);

    // logger::info("Calibrating BMP390");
    // int n = 500;
    // for(int i = 0; i < n; i ++){
    //     bmp390::read_fifo_wo_height(bmp390_data);
    //     bmp390_data[1] = pressure_filter[bmp390_data[1]];
    //     bmp390_data[2] = bmp390::get_height(bmp390_data[0], bmp390_data[1]);

    //     usleep(sensor_sleep_int);
    // }

    // double sum = 0;
    // for(int i = 0; i < n; i ++){
    //     bmp390::read_fifo(bmp390_data);
    //     sum += bmp390_data[2];
    //     usleep(sensor_sleep_int);
    // }

    // initial_altitude = old_altitude = sum / n;

    logger::info("Calibrated sensors");
    curr_state = old;
}

void sensor_thread_funct(){
    double dt = sensor_timer.dt;
    
    // logger::info("dt: {:10f} ms",dt * 1000);

    { // MPU6050 Sensor Read & Filter
        mpu6050::read(mpu6050_data);
        mpu6050_data[4] *= -1;
        mpu6050_data[5] *= -1;
        for(int i = 0; i < 6; i ++){
            filtered_mpu6050_data[i] = mpu6050_filters[i][mpu6050_data[i]];
        }
    }

    { // BMP390 Sensor Read & Filter
        old_altitude = bmp390_data[2];
        // bmp390::read(bmp390_data);
        // bmp390_data[1] = pressure_filter[bmp390_data[1]];
        // bmp390_data[2] = bmp390::get_height(bmp390_data[0], bmp390_data[1]);
        bmp390_data[0] = bmp390::get_temp();
        bmp390_data[1] = bmp390::get_press(bmp390_data[0]);
        bmp390_data[1] = pressure_filter[bmp390_data[1]];
        bmp390_data[2] = bmp390::get_height(bmp390_data[0], bmp390_data[1]);
        valt = (bmp390_data[2] - old_altitude) / dt;
    }

    {// Dead Reckoning
        math::vector euler_v = math::vector(filtered_mpu6050_data[3]*dt*DEG_TO_RAD, filtered_mpu6050_data[4]*dt*DEG_TO_RAD, filtered_mpu6050_data[5]*dt*DEG_TO_RAD);
        math::quarternion euler_q = math::quarternion::fromEulerZYX(euler_v);
        orientation = euler_q*orientation;
        orientation_euler = math::quarternion::toEuler(orientation);


        double a_dist_from_one_sqrd = mpu6050_data[0] * mpu6050_data[0] + mpu6050_data[1] * mpu6050_data[1] + mpu6050_data[2] * mpu6050_data[2] - 1;
        if((a_dist_from_one_sqrd < 0 ? a_dist_from_one_sqrd > - sensor_g_tolerance_sqrd : a_dist_from_one_sqrd < sensor_g_tolerance_sqrd)){
            double roll = atan2(mpu6050_data[1], mpu6050_data[2]);
            double pitch = atan2((mpu6050_data[0]) , sqrt(mpu6050_data[1] * mpu6050_data[1] + mpu6050_data[2] * mpu6050_data[2]));

            orientation_euler.x = orientation_euler.x * (1 - sensor_roll_pitch_tau) + -roll * sensor_roll_pitch_tau;
            orientation_euler.y = orientation_euler.y * (1 - sensor_roll_pitch_tau) + -pitch * sensor_roll_pitch_tau;
            
            logger::info("{:.4f} {:.4f} | {:.4f} {:.4f}", orientation_euler.x * RAD_T_DEG, -roll * RAD_T_DEG, orientation_euler.y * RAD_T_DEG, -pitch * RAD_T_DEG);

            orientation = math::quarternion::fromEulerZYX(orientation_euler);
            orientation_euler = math::quarternion::toEuler(orientation);
        }

        math::vector temp = velocity * dt;
        position = position + temp;
        position.z = position.z * sensor_z_tau + bmp390_data[2] * (1 - sensor_z_tau);
        temp = math::vector(filtered_mpu6050_data[0]*dt*G, filtered_mpu6050_data[1]*dt*G, -filtered_mpu6050_data[2]*dt*G);
        temp = math::quarternion::rotateVector(orientation, temp);
        temp.z += G*dt;
        velocity = velocity + temp;
        velocity.z = vzfilter[velocity.z];
        velocity.z = velocity.z * (1 - sensor_z_tau) + valt * sensor_z_tau;
    }

    {// PID updates
        // z_controller.setpoint = 
        // double z = z_controller.update(position.z, dt);
        double z = 0;
        double r = roll_controller.update(orientation_euler.x, dt);
        double p = pitch_controller.update(orientation_euler.y, dt);
        // double vy = vyaw_controller.update(filtered_mpu6050_data[5], dt);
        double vy = yawthrust;

        // logger::info("p: {:.4f} o: {:.4f}", roll_controller.p, roll_controller.output);

        drone::set_motor(MOTOR_FL, (z + r + p + vy + trim + thrust) * front_multiplier);
        drone::set_motor(MOTOR_FR, (z - r + p - vy + trim + thrust) * front_multiplier);
        drone::set_motor(MOTOR_BL, z + r - p - vy + trim + thrust);
        drone::set_motor(MOTOR_BR, z - r - p + vy + trim + thrust);
    }

    if(zero_flag){
        orientation = math::quarternion(1, 0, 0, 0);

        velocity = math::vector(0, 0, 0);
        position = math::vector(0, 0, 0);
        initial_altitude = old_altitude = bmp390_data[2];
        valt = 0;

        logger::info("Zeroed");
        zero_flag = false;
    }

    if(calib_flag){
        calibrate();
        settle();
        calib_flag = false;
    }
}

void reload_config_thread(){
    std::lock_guard<std::mutex> sensor_lock_guard(sensor_thread_mutex);
    std::lock_guard<std::mutex> message_lock_guard(message_thread_mutex);

    logger::info("Acquired locks!");
    drone::load_configuration();
    logger::info("Releasing locks!");
    sensor_timer.interval = 1000 / sensor_ref_rate;
}


void reconnect_node_server(sock::socket& client, sock::un_connection& unix_connection){
    logger::crit("Lost contact with node server. \"{}\"", socket_path);
    if(unix_connection.valid){
        unix_connection.close();
    }

    while(alive && !unix_connection.valid) {
        client.close();
        client = sock::socket(sock::unix, sock::tcp);
        unix_connection = client.un_connect(socket_path.c_str());  
        usleep(100000);
    }
    logger::crit("Found node server!");
}


static int substr_chr(char * out, const char * bruh, char c, int a, int b){
    int k = 0;
    for(int i = a; i != b && bruh[i] != '\0'; i ++){
        if(bruh[i] == c){
            out[k++] = '\0';
            return i;
        }
        out[k++] = bruh[i];
    }
    out[k] = '\0';
    return -1;
}


// for string delimiter
static std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


void message_thread_funct(){
    logger::info("Message thread alive!");  

    {
        parameters::bind_dbl("ax", filtered_mpu6050_data, true); // 0
        parameters::bind_dbl("ay", filtered_mpu6050_data + 1, true); // 1
        parameters::bind_dbl("az", filtered_mpu6050_data + 2, true); // 2
        parameters::bind_dbl("vroll", filtered_mpu6050_data + 3, true); // 3
        parameters::bind_dbl("vpitch", filtered_mpu6050_data + 4, true); // 4
        parameters::bind_dbl("vyaw", filtered_mpu6050_data + 5, true); // 5
        parameters::bind_dbl("vx", &velocity.x, true); // 6
        parameters::bind_dbl("vy", &velocity.y, true); // 7
        parameters::bind_dbl("vz", &velocity.z, true); // 8
        parameters::bind_dbl("x", &position.x, true); // 9
        parameters::bind_dbl("y", &position.y, true); // 10
        parameters::bind_dbl("z", &position.z, true); // 11
        parameters::bind_dbl("roll", &orientation_euler.x, true); // 12
        parameters::bind_dbl("pitch", &orientation_euler.y, true); // 13
        parameters::bind_dbl("yaw", &orientation_euler.z, true); // 14
        parameters::bind_dbl("temp", bmp390_data, true); // 15
        parameters::bind_dbl("pressure", bmp390_data+1, true); // 16
        parameters::bind_dbl("altitude", bmp390_data+2, true); // 17
        parameters::bind_dbl("ialt", &initial_altitude, true); // 18
        parameters::bind_dbl("valt", &valt, true); // 19
        
        parameters::bind_dbl("zset", &z_controller.setpoint, true); // 20
        parameters::bind_dbl("vyawset", &vyaw_controller.setpoint, true); // 21
        parameters::bind_dbl("rollset", &roll_controller.setpoint, true); // 22
        parameters::bind_dbl("pitchset", &pitch_controller.setpoint, true); // 23
        parameters::bind_dbl("zerr", &z_controller.err, true); // 24
        parameters::bind_dbl("vyawerr", &vyaw_controller.err, true); // 25
        parameters::bind_dbl("rollerr", &roll_controller.err, true); // 26
        parameters::bind_dbl("pitcherr", &pitch_controller.err, true); // 27

        parameters::bind_dbl("flpwr", &motor_fl_spd, true); // 28
        parameters::bind_dbl("frpwr", &motor_fr_spd, true); // 29
        parameters::bind_dbl("blpwr", &motor_bl_spd, true); // 30
        parameters::bind_dbl("brpwr", &motor_br_spd, true); // 31

        parameters::bind_int("state", (int *) (&curr_state), true); // 32
        parameters::bind_dbl("cpuusg", &cpu_usg, true); // 33
        parameters::bind_dbl("battery", &battery, true); // 34
        parameters::bind_dbl("dt", &sensor_timer.dt, true); // 35
        parameters::bind_bool("controller", &cntrller_connected, true); // 36

        parameters::bind_dbl("zicurr", &z_controller.i_curr, true); // 37
        parameters::bind_dbl("vyawicurr", &vyaw_controller.i_curr, true); // 38
        parameters::bind_dbl("rollicurr", &roll_controller.i_curr, true); // 39
        parameters::bind_dbl("pitchicurr", &pitch_controller.i_curr, true); // 40
        parameters::bind_dbl("zderr", &z_controller.derr, true); // 41
        parameters::bind_dbl("vyawderr", &vyaw_controller.derr, true); // 42
        parameters::bind_dbl("rollderr", &roll_controller.derr, true); // 43
        parameters::bind_dbl("pitchderr", &pitch_controller.derr, true); // 44
        parameters::bind_dbl("zp", &z_controller.p, true); // 45
        parameters::bind_dbl("vyawp", &vyaw_controller.p, true); // 46
        parameters::bind_dbl("rollp", &roll_controller.p, true); // 47
        parameters::bind_dbl("pitchp", &pitch_controller.p, true); // 48
        parameters::bind_dbl("zi", &z_controller.i, true); // 49
        parameters::bind_dbl("vyawi", &vyaw_controller.i, true); // 50
        parameters::bind_dbl("rolli", &roll_controller.i, true); // 51
        parameters::bind_dbl("pitchi", &pitch_controller.i, true); // 52
        parameters::bind_dbl("zd", &z_controller.d, true); // 53
        parameters::bind_dbl("vyawd", &vyaw_controller.d, true); // 54
        parameters::bind_dbl("rolld", &roll_controller.d, true); // 55
        parameters::bind_dbl("pitchd", &pitch_controller.d, true); // 56
        parameters::bind_dbl("zout", &z_controller.output, true); // 57
        parameters::bind_dbl("vyawout", &vyaw_controller.output, true); // 58
        parameters::bind_dbl("rollout", &roll_controller.output, true); // 59
        parameters::bind_dbl("pitchout", &pitch_controller.output, true); // 60

        parameters::bind_dbl("trim", &trim, false); // 61
        parameters::bind_dbl("fmult", &front_multiplier, false);

        parameters::bind_dbl("rkP", &roll_controller.kP, false);
        parameters::bind_dbl("rkI", &roll_controller.kI, false);
        parameters::bind_dbl("rkD", &roll_controller.kD, false);
        parameters::bind_dbl("pkP", &pitch_controller.kP, false);
        parameters::bind_dbl("pkI", &pitch_controller.kI, false);
        parameters::bind_dbl("pkD", &pitch_controller.kD, false);
        parameters::bind_dbl("vyawkP", &vyaw_controller.kP, false);
        parameters::bind_dbl("vyawkI", &vyaw_controller.kI, false);
        parameters::bind_dbl("vyawkD", &vyaw_controller.kD, false);
        parameters::bind_dbl("zkP", &z_controller.kP, false);
        parameters::bind_dbl("zkI", &z_controller.kI, false);
        parameters::bind_dbl("zkD", &z_controller.kD, false);

        parameters::bind_dbl("sensorRollPitchTau", &sensor_roll_pitch_tau, false);
        parameters::bind_dbl("sensorZTau", &sensor_z_tau, false);

        parameters::bind_dbl("axis0", gamepad::get_axis_ptr(0), false);
        parameters::bind_dbl("axis1", gamepad::get_axis_ptr(1), false);
        parameters::bind_dbl("axis2", gamepad::get_axis_ptr(2), false);
        parameters::bind_dbl("axis3", gamepad::get_axis_ptr(3), false);
        parameters::bind_dbl("axis4", gamepad::get_axis_ptr(4), false);
        parameters::bind_dbl("axis5", gamepad::get_axis_ptr(5), false);
        parameters::bind_dbl("axis6", gamepad::get_axis_ptr(6), false);
        parameters::bind_dbl("axis7", gamepad::get_axis_ptr(7), false);
    }
    sock::socket client(sock::unix, sock::tcp);
    sock::un_connection unix_connection = client.un_connect(socket_path.c_str());

    if(!unix_connection.valid){
        reconnect_node_server(client, unix_connection);
    }
    char send[1024];
    char recv[1024];
    char buf[100];

    while(alive){
        // std::lock_guard<std::mutex> message_lock_guard(message_thread_mutex);
        

        // | zero | calibrate | Reload Config | Chg PID Config |
        // |  0   |     1     |       2       |         3         |
        if(unix_connection.can_read()){
            logger::info("YOO DATA!");
            int len = unix_connection.read(recv, 50);
            logger::info("{}", recv);
            recv[len] = '\0';
            std::string bruv(recv);
            if(len > 0){
                std::vector<std::string> args = split(bruv, " ");
                int cmd = std::stoi(args[0]);
                state old;
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
                    old = curr_state;
                    curr_state = state::configuring;
                    logger::info("Reloading configuration");
                    logger::info("Acquired locks!");
                    drone::load_configuration();
                    logger::info("Releasing locks!");
                    curr_state = old;
                    break;
                case 3:
                    parameters::chg(args, 1);
                    break;
                default:
                    logger::warn("Unknown cmd \"{}\"", cmd);
                }
            }else{
                reconnect_node_server(client, unix_connection);
            }
            // logger::info("Message: \"{}\"", recv);
        }

        std::string sendStr = parameters::get_json_report();
        int e = unix_connection.send(sendStr.c_str(), sendStr.length());
        if(e < 0) {
            reconnect_node_server(client, unix_connection);
        }



        usleep(message_sleep_int);
    }
}

void drone::init_sensors(bool thread) {
    curr_state = state::init;
    
    {
        logger::info("Initializing MPU6050.");
        mpu6050::init();
        mpu6050::set_accl_set(mpu6050::accl_range::g_2);
        mpu6050::set_gyro_set(mpu6050::gyro_range::deg_250);
        mpu6050::set_clk(mpu6050::clk::y_gyro);
        mpu6050::set_fsync(mpu6050::fsync::input_dis);
        mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
        mpu6050::wake_up();
        logger::info("Finished intializing the MPU6050.");
    }
    {
        logger::info("Initializing BMP390.");
        bmp390::init();
        bmp390::soft_reset();
        bmp390::set_oversample(bmp390::oversampling::STANDARD, bmp390::ULTRA_LOW_POWER);
        bmp390::set_iir_filter(bmp390::COEFF_3);
        bmp390::set_output_data_rate(bmp390::hz50);
        bmp390::set_enable(true, true);
        
        bmp390::set_enable_fifo(true, true);
        bmp390::set_fifo_stop_on_full(false);

        bmp390::set_pwr_mode(bmp390::NORMAL);
        logger:info("Finished initializing the BMP390.");
    }

    if(thread){
        logger::info("Starting up sensor thread.");

        // orientation = math::quarternion(1, 0, 0, 0);

        math::quarternion euler_q;
        math::vector euler_v;
        math::vector temp;

        auto then = std::chrono::steady_clock::now();
        auto start = then;
        auto now = std::chrono::steady_clock::now();

        setup_filters();
        calibrate();
        settle();
        
        curr_state = state::ready;
        logger::info("Sleep interval: {} ms", 1000 / sensor_ref_rate);
        sensor_timer.command = sensor_thread_funct;
        sensor_timer.start();
        // sensor_timer.start(sensor_thread_funct, 1000 / sensor_ref_rate);
        // sensor_thread = std::thread(sensor_thread_funct);
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
    if(message_thread.joinable()){
        logger::info("Joining message thread.");
        alive = false;
        message_thread.join();
        logger::info("Joined message thread.");
    }
    if(rel_config.joinable()){
        rel_config.join();
    }
}

void drone::destroy_sensors(){
    curr_state = state::destroying;
    logger::info("Joining sensor thread.");
    alive = false;
    
    sensor_timer.stop();

    logger::info("Joined sensor thread.");
    
}


void drone::set_trim(double _trim){
    trim = _trim;
}

double drone::get_trim(){
    return trim;
}

double* drone::get_trim_ptr(){
    return &trim;
}
double* drone::get_thrust_ptr(){
    return &thrust;
}
double* drone::get_yawthrust_ptr(){
    return &yawthrust;
}

double* drone::get_front_multiplier_ptr(){
    return &front_multiplier;
}
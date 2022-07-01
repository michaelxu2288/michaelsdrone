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



math::quarternion orientation;
math::vector orientation_euler;
math::vector position;


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

void drone::set_all(double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
    pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
    pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
    pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
}

void drone::set_diagonals(short diagonal, double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    switch (diagonal)
    {
    case FLBR_DIAGONAL:
        pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
        
        break;
    case FRBL_DIAGONAL:
        pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
        pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        break;
    
    default:
        break;
    }
}

void drone::set_motor(short motor, double per){
    int pow = (int)(per * (THROTTLE_MAX - THROTTLE_MIN)) + THROTTLE_MIN;
    switch(motor){
    case MOTOR_FL:
        pca9685::set_pwm_ms(MOTOR_FL_PIN, pow);
        break;
    case MOTOR_FR:
        pca9685::set_pwm_ms(MOTOR_FR_PIN, pow);
        break;
    case MOTOR_BL:
        pca9685::set_pwm_ms(MOTOR_BL_PIN, pow);
        break;
    case MOTOR_BR:
        pca9685::set_pwm_ms(MOTOR_BR_PIN, pow);
        break;
    default:break;
    }
}



double throttle = 0.0;

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


static std::thread sensor_thread;
static int mpu6050_ref_rate;

static bool sensor_loop_alive = true;

void sensor_thread_funct(){
    int sleep_int = 1000000 / ref_rate;
    double data[6];
    
    orientation = math::quarternion(1, 0, 0, 0);

    math::quarternion euler_q;
    math::vector euler_v;

    auto then = std::chrono::steady_clock::now();
    auto start = then;
    auto now = std::chrono::steady_clock::now();

    while(sensor_loop_alive){

        mpu6050::read(data);
        now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::milliseconds> (now - then).count() * 0.001;
        t_since = std::chrono::duration_cast<std::chrono::milliseconds> (now - start).count();
        then = now;

        euler_v = math::vector(data[3]*dt*DEG_TO_RAD, data[4]*dt*DEG_TO_RAD, data[5]*dt*DEG_TO_RAD);
        euler_q = math::quarternion::fromEulerZYX(euler_v);
        rotation = euler_q*rotation;

        orientation_euler = math::quarternion::toEuler(rotation);

        usleep(sleep_int);
    }
}



void drone::init_sensors() {
    logger::info("Loading sensor configuration.");
    config::load_file();
    
    mpu6050_ref_rate = config::get_config_int("mpu6050_ref_rate", 60);
    
    config::write_to_file();
    logger::info("Finished loading sensor configuration.");
    
    logger::info("MPU6050 Refresh Rate: {}hz ", mpu6050_ref_rate);


    logger::info("Initializing MPU6050.");
    mpu6050::init();
    mpu6050::set_accl_set(mpu6050::accl_range::g_2);
    mpu6050::set_gyro_set(mpu6050::gyro_range::deg_2000);
    mpu6050::set_clk(mpu6050::clk::y_gyro);
    mpu6050::set_fsync(mpu6050::fsync::input_dis);
    mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
    mpu6050::wake_up();
    logger::info("Finished intializing the MPU6050.");

    logger::info("Starting up sensor thread.");
    sensor_thread = std::thread(sensor_thread_funct);
}


void drone::destroy_sensors(){

    logger::info("Joining sensor thread.");
    sensor_loop_alive = false;
    sensor_thread..join();
    logger::info("Joined sensor thread.");


}
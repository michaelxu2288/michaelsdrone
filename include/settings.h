#ifndef SETTINGS_H
#define SETTINGS_H
#define MOTOR_FL 1
#define MOTOR_FR 2
#define MOTOR_BL 3
#define MOTOR_BR 4

#define PWM_FULL 19999

#define MOTOR_FL_PIN 0
#define MOTOR_FR_PIN 1
#define MOTOR_BL_PIN 2
#define MOTOR_BR_PIN 3

#define LED_RUN_PIN 15
#define LED_ERROR_PIN 14
#define LED_AUTO_PIN 11
#define LED_WHITE_PIN 13
#define LED_BLUE_PIN 12

#define ENABLE_FL 1
#define ENABLE_FR 1
#define ENABLE_BL 1
#define ENABLE_BR 1

#define FLBR_DIAGONAL 1
#define FRBL_DIAGONAL 2

#define THROTTLE_MIN 1000 // throttle minimum pwm 1 ms
#define THROTTLE_MAX 2000 // throttle maximum pwm 2 ms

#define DEFAULT_CONFIG_FILE "/home/pi/drone/config/config.json"
#define DRONE_STATE_CONFIG_FILE "/home/pi/drone/run/drone.json"

#define ACCEL_X_VARIANCE 0.00003
#define ACCEL_Y_VARIANCE 0.00003
#define ACCEL_Z_VARIANCE 0.00006
#define ACCEL_VARIANCE 0.00012
#define GYRO_X_VARIANCE 0.03
#define GYRO_Y_VARIANCE 0.03
#define GYRO_Z_VARIANCE 0.06
#define GYRO_VARIANCE 0.12



#define TEMP_VARIANCE 0.0003
#define PRESS_VARIANCE 25.0
#define ALTITUDE_VARIANCE 0.2


// UNCOMMENT THIS TO ENABLE MOTOR
// #define ENABLE_MOTOR

#endif


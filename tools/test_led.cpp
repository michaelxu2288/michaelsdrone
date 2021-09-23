#include <settings.h>
#include <pca9685.h>
#include <unistd.h>

int main(){

    pca9685::init();
    pca9685::set_frequency(50);
    pca9685::wake_up();
    pca9685::set_pwm_ms(LED_RUN_PIN, PWM_FULL);
    pca9685::set_pwm_ms(LED_AUTO_PIN, PWM_FULL);
    pca9685::set_pwm_ms(LED_WHITE_PIN, PWM_FULL);
    pca9685::set_pwm_ms(LED_BLUE_PIN, PWM_FULL);
    pca9685::set_pwm_ms(LED_ERROR_PIN, PWM_FULL);
    usleep(10000000);
    pca9685::set_pwm_ms(LED_RUN_PIN, 0);
    pca9685::set_pwm_ms(LED_AUTO_PIN, 0);
    pca9685::set_pwm_ms(LED_WHITE_PIN, 0);
    pca9685::set_pwm_ms(LED_BLUE_PIN, 0);
    pca9685::set_pwm_ms(LED_ERROR_PIN, 0);
    pca9685::sleep();
    pca9685::destroy();
}
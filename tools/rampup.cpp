#include <pwm.h>
#include <iostream>
#include <unistd.h>
#include <settings.h>



int main(){

    int cycle = 1100;

    int maximum_speed = 1500;

    std::cout << "Setting all motors to a very low speed\n";
    std::cout << "PWM signal = " << (cycle / 1000.0) << "ms\n";
    std::cout << "PWM period = 20 ms";
    pwm fl(MOTOR_FL_PIN);//, fr(MOTOR_FR_PIN), bl(MOTOR_BL_PIN), br(MOTOR_BR_PIN);

    fl.pulse_width = cycle;
    // fr.pulse_width = cycle;
    // bl.pulse_width = cycle;
    // br.pulse_width = cycle;

    fl.start();
    // fr.start();
    // bl.start();
    // br.start();

    std::cout << "Check each motor on opposite diagonals are spinning in the same direction\n and each motor on the opposite side are spinning in the other direction\n";

    std::cout << "Waiting 10 seconds\n";
    for(int i = 1; i <= 1000; i ++){
        usleep(100000);
        fl.pulse_width = (maximum_speed - THROTTLE_MIN) * i / 1000 + THROTTLE_MIN;
        std::cout << i << "\n";
    }

    fl.stop();
    // fr.stop();
    // bl.stop();
    // br.stop();

    fl.close();
    // fr.close();
    // bl.close();
    // br.close();

    std::cout << "Motors should have rotated on success";

}
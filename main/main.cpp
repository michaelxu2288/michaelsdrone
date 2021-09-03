#include <iostream>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <ctime>
#include <i2c.h>
#include <MPU6050.h>

#define GYRO_SENS 131.0
#define GYRO_CONFIG 0b00000000

#define ACCEL_SENS 16384.0
#define ACCEL_CONFIG 0b00000000






float Ax = 0, Ay = 0, Az = 0, Gx = 0, Gy = 0, Gz = 0;

void inline print_drone_data(){
    printf("\n%10.10f %10.10f %10.10f", Ax, Ay, Az);
}




void main_io(){

    while(true){
        print_drone_data();
        usleep(100000);
    }

}
const int accel_addr = 0x68;

/*
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
*/
void accel_io(){
    // I2C::Device IMU(accel_addr);
    I2C::start();

    I2C::select_slave(accel_addr);

    I2C::write_byte(0x6b, 0b00000000);
    I2C::write_byte(0x1a, 0b00000011);
    I2C::write_byte(0x19, 0b00000100);
    I2C::write_byte(0x1b, GYRO_CONFIG);
    I2C::write_byte(0x1c, ACCEL_CONFIG);
    I2C::write_byte(0x06, 0b00000000);
    I2C::write_byte(0x07, 0b00000000);
    I2C::write_byte(0x08, 0b00000000);
    I2C::write_byte(0x09, 0b00000000);
    I2C::write_byte(0x0A, 0b00000000); 
    I2C::write_byte(0x0B, 0b00000000);
    I2C::write_byte(0x00, 0b10000001);
    I2C::write_byte(0x01, 0b00000001);
    I2C::write_byte(0x02, 0b10000001);

    unsigned char buffer[32];
    while(true){
        //I2C::read(MPU6050_RA_ACCEL_XOUT_H, buffer, 6);

        Ax = ((float)(I2C::read_byte(0x3b) << 8 | I2C::read_byte(0x3c))) / ACCEL_SENS - 1.8585205078f; //Read X registers
	    Ay = ((float)(I2C::read_byte(0x3d) << 8 | I2C::read_byte(0x3e))) / ACCEL_SENS - 3.5607910156f; //Read Y registers
	    Az = ((float)(I2C::read_byte(0x3f) << 8 | I2C::read_byte(0x40))) / ACCEL_SENS - 0.0604248047f; 
        // Ax = ((float)(I2C::read_byte(0x43) << 8 | I2C::read_byte(0x44))) / GYRO_SENS - 2.3511450291f; //Read X registers
	    // Ay = ((float)(I2C::read_byte(0x45) << 8 | I2C::read_byte(0x46))) / GYRO_SENS - 495.8244323730f; //Read Y registers
	    // Az = ((float)(I2C::read_byte(0x47) << 8 | I2C::read_byte(0x48))) / GYRO_SENS - 498.0534362793f; 
        usleep(100000);
    }
}

std::thread main_io_thread;
std::thread accel_thread;

int main(){
    setvbuf(stdout, NULL,_IONBF,0);
    srand((unsigned) time(0));

    main_io_thread = std::thread(main_io);
    accel_thread = std::thread(accel_io);

    while(true);

    main_io_thread.join();
}
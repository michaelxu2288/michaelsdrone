// A tool to calibrate some variables.

#include <MPU6050.h>
#include <cstdlib>

// Gets offsets for the mpu6050 accelerometer.
void mpu6050_calibration(){

    MPU6050 sensor(0x68, false);

    printf("-------------- MPU6050 CALIBRATION --------------\n");
    float o_ax, o_ay, o_az, o_gx, o_gy, o_gz;
    sensor.getOffsets(&o_ax,&o_ay,&o_az,&o_gx,&o_gy,&o_gz);
    printf("Offset Accl X: %10f\nOffset Accl Y: %10f\nOffset Accl Z: %10f\nOffset Gyro X: %10f\nOffset Gyro Y: %10f\nOffset Gyro Z: %10f\n", o_ax, o_ay, o_az, o_gz, o_gy, o_gz);
}







int main(){
    printf("\n\n\n");
    mpu6050_calibration();
    printf("\n\n\n");
}












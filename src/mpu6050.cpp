//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Copyright (c) 2019, Alex Mous
//Licensed under the CC BY-NC SA 4.0

// MODIFIED BY ANDREW LIN

//Include the header file for this class
#include "mpu6050.h"


#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
extern "C" {
	#include <linux/i2c-dev.h>
	#include <i2c/smbus.h>
}
#include <cmath>
#include <thread>
#include <cstdio>
#include <string>
#include <cstdint>

#define Read(r) i2c_smbus_read_byte_data(fd, r)
#define Write(r,v) i2c_smbus_write_byte_data(fd, r, v)

static int fd;

void mpu6050::init(){init(MPU6050_DEFAULT_ADDR);}

static int offsets[6];

static int accl_scale = 16384;
static double gyro_scale = 16.4;

void mpu6050::init(int addr){
    fd = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
	if (fd < 0) { //Catch errors
		std::cout << "ERR (mpu6050.cpp:open()): Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config\n"; //Print error message
	}

	int status = ioctl(fd, I2C_SLAVE, addr); //Set the I2C bus to use the correct address
	if (status < 0) {
		std::cout << "ERR (mpu6050.cpp:open()): Could not get I2C bus with " << addr << " address. Please confirm that this address is correct\n"; //Print error message
	}
	
	offsets[0] = X_ACCL_SHIFT;
	offsets[1] = Y_ACCL_SHIFT;
	offsets[2] = Z_ACCL_SHIFT;
	offsets[3] = X_GYRO_SHIFT;
	offsets[4] = Y_GYRO_SHIFT;
	offsets[5] = Z_GYRO_SHIFT;
}


inline void debug (int reg){
	printf("[DEBUG] Value of register %2x: %5d\n", reg, Read(reg));
}

inline void debug2 (const char * name, int reg){
	printf("[DEBUG] Register %20s: Value of register %2x: %5d\n", name, reg, Read(reg));
}

inline void debug3(int val){
	printf("[DEBUG] Changing value to %5d", val);
}

void mpu6050::print_debug(){
	debug2("Pwr Mng 1",REG_PWR_MNG_1);
	debug2("Cfg", REG_CFG);
	debug2("Accl Cfg", REG_ACCL_CFG);
	debug2("Gyro Cfg", REG_GYRO_CFG);

}

void mpu6050::wake_up(){
	Write(REG_PWR_MNG_1, Read(REG_PWR_MNG_1) & (~0b01000000));
	usleep(1000);
	debug(REG_PWR_MNG_1);
}

void mpu6050::sleep(){
	Write(REG_PWR_MNG_1, Read(REG_PWR_MNG_1) | 0b01000000);
	usleep(1000);
	debug(REG_PWR_MNG_1);
}

void mpu6050::set_accl_set(accl_range::accl_range set){
	switch(set){
	case accl_range::g_16:
    	accl_scale = 2048;
		break;
	case accl_range::g_8: 
    	accl_scale = 4096;
		break;
	case accl_range::g_4: 
    	accl_scale = 8192;
		break;
	case accl_range::g_2: 
    	accl_scale = 16384;
		break;
	}
	Write(REG_ACCL_CFG, Read(REG_ACCL_CFG) & (~0b00011000) | (set << 3));
	usleep(1000);
	debug(REG_ACCL_CFG);
}

void mpu6050::set_gyro_set(gyro_range::gyro_range set){
	switch(set){
	case gyro_range::deg_250:
		gyro_scale = 131;
		break;
	case gyro_range::deg_500:
		gyro_scale = 65.5;
		break;
	case gyro_range::deg_1000:
		gyro_scale = 32.8;
		break;
	case gyro_range::deg_2000:
		gyro_scale = 16.4;
		break;
	}
	Write(REG_GYRO_CFG, Read(REG_GYRO_CFG) & (~0b00011000) | (set << 3));
	usleep(1000);
	debug(REG_GYRO_CFG);


}

void mpu6050::set_clk(clk::clk set){
	Write(REG_PWR_MNG_1, Read(REG_PWR_MNG_1) & (~0b00000111) | set);
	usleep(1000);
	debug(REG_PWR_MNG_1);
}

void mpu6050::set_dlpf_bandwidth(dlpf::dlpf set){
	Write(REG_CFG, Read(REG_CFG) & (~0b00000111) | set);
	
	usleep(1000);
	debug(REG_CFG);
}

void mpu6050::set_fsync(fsync::fsync set){
	Write(REG_CFG, Read(REG_CFG) & (~0b00111000) | (set << 3));
	usleep(1000);
	debug(REG_CFG);
}

void mpu6050::set_pwr_set(int set){

}

int handle_neg(int n){
	if(n & 0x4000){
		return -((~(n - 1))&0x7FFF);
	}
	return n&0x7FFF;
}

void mpu6050::read_raw(int * data){
	data[0] = handle_neg(Read(OUT_XACCL_H) << 8 | Read(OUT_XACCL_L));
	data[1] = handle_neg(Read(OUT_YACCL_H) << 8 | Read(OUT_YACCL_L));
	data[2] = handle_neg(Read(OUT_ZACCL_H) << 8 | Read(OUT_ZACCL_L));
	data[3] = handle_neg(Read(OUT_XGYRO_H) << 8 | Read(OUT_XGYRO_L));
	data[4] = handle_neg(Read(OUT_YGYRO_H) << 8 | Read(OUT_YGYRO_L));
	data[5] = handle_neg(Read(OUT_ZGYRO_H) << 8 | Read(OUT_ZGYRO_L));
}

void mpu6050::read_wo_offsets(double * data){
	data[0] = ((double) (handle_neg(Read(OUT_XACCL_H) << 8 | Read(OUT_XACCL_L)))) / accl_scale;
	data[1] = ((double) (handle_neg(Read(OUT_YACCL_H) << 8 | Read(OUT_YACCL_L)))) / accl_scale;
	data[2] = ((double) (handle_neg(Read(OUT_ZACCL_H) << 8 | Read(OUT_ZACCL_L)))) / accl_scale;
	data[3] = ((double) (handle_neg(Read(OUT_XGYRO_H) << 8 | Read(OUT_XGYRO_L)))) / gyro_scale;
	data[4] = ((double) (handle_neg(Read(OUT_YGYRO_H) << 8 | Read(OUT_YGYRO_L)))) / gyro_scale;
	data[5] = ((double) (handle_neg(Read(OUT_ZGYRO_H) << 8 | Read(OUT_ZGYRO_L)))) / gyro_scale;
}


void mpu6050::read(double * data){
	data[0] = ((double) (handle_neg(Read(OUT_XACCL_H) << 8 | Read(OUT_XACCL_L))) - offsets[0]) / accl_scale;
	data[1] = ((double) (handle_neg(Read(OUT_YACCL_H) << 8 | Read(OUT_YACCL_L))) - offsets[1]) / accl_scale;
	data[2] = ((double) (handle_neg(Read(OUT_ZACCL_H) << 8 | Read(OUT_ZACCL_L))) - offsets[2]) / accl_scale;
	data[3] = ((double) (handle_neg(Read(OUT_XGYRO_H) << 8 | Read(OUT_XGYRO_L))) - offsets[3]) / gyro_scale;
	data[4] = ((double) (handle_neg(Read(OUT_YGYRO_H) << 8 | Read(OUT_YGYRO_L))) - offsets[4]) / gyro_scale;
	data[5] = ((double) (handle_neg(Read(OUT_ZGYRO_H) << 8 | Read(OUT_ZGYRO_L))) - offsets[5]) / gyro_scale;
}

void read_int(int * data){
	data[0] = ((handle_neg(Read(OUT_XACCL_H) << 8 | Read(OUT_XACCL_L))) - offsets[0]);
	data[1] = ((handle_neg(Read(OUT_YACCL_H) << 8 | Read(OUT_YACCL_L))) - offsets[1]);
	data[2] = ((handle_neg(Read(OUT_ZACCL_H) << 8 | Read(OUT_ZACCL_L))) - offsets[2]);
	data[3] = ((handle_neg(Read(OUT_XGYRO_H) << 8 | Read(OUT_XGYRO_L))) - offsets[3]);
	data[4] = ((handle_neg(Read(OUT_YGYRO_H) << 8 | Read(OUT_YGYRO_L))) - offsets[4]);
	data[5] = ((handle_neg(Read(OUT_ZGYRO_H) << 8 | Read(OUT_ZGYRO_L))) - offsets[5]);
}


int mpu6050::query_register(int reg){
	return Read(reg);
}

void mpu6050::set_register(int reg, int data){
	Write(reg,data);
}



void mpu6050::calibrate(int n){
	
	int mean_ax = 0, mean_ay = 0, mean_az = 0, mean_gx = 0, mean_gy = 0, mean_gz = 0;

	int ax_offset=-mean_ax/8;
	int ay_offset=-mean_ay/8;
	int az_offset=(16384-mean_az)/8;

	int gx_offset=-mean_gx/4;
	int gy_offset=-mean_gy/4;
	int gz_offset=-mean_gz/4;
	int readings[6];

	int acel_deadzone=8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
	int giro_deadzone=1;

	while (1){
		int ready=0;
		set_offsets(ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset);


		// meansensors();
		// Serial.println("...");

		for(int i = 0; i < 1000; i ++){
			read_int(readings);
			mean_ax += readings[0];
			mean_ay += readings[1];
			mean_az += readings[2];
			mean_gx += readings[3];
			mean_gy += readings[4];
			mean_gz += readings[5];
			usleep(100);
		}

		mean_ax /= 1000;
		mean_ay /= 1000;
		mean_az /= 1000;
		mean_gx /= 1000;
		mean_gy /= 1000;
		mean_gz /= 1000;

		if (abs(mean_ax)<=acel_deadzone) ready++;
		else ax_offset=ax_offset-mean_ax/acel_deadzone;

		if (abs(mean_ay)<=acel_deadzone) ready++;
		else ay_offset=ay_offset-mean_ay/acel_deadzone;

		if (abs(16384-mean_az)<=acel_deadzone) ready++;
		else az_offset=az_offset+(16384-mean_az)/acel_deadzone;

		if (abs(mean_gx)<=giro_deadzone) ready++;
		else gx_offset=gx_offset-mean_gx/(giro_deadzone+1);

		if (abs(mean_gy)<=giro_deadzone) ready++;
		else gy_offset=gy_offset-mean_gy/(giro_deadzone+1);

		if (abs(mean_gz)<=giro_deadzone) ready++;
		else gz_offset=gz_offset-mean_gz/(giro_deadzone+1);

		if (ready==6) break;
	}
}

void mpu6050::set_offsets(int x_a, int y_a, int z_a, int x_g, int y_g, int z_g){
	offsets[0] = x_a;
	offsets[1] = y_a;
	offsets[2] = z_a;
	offsets[3] = x_g;
	offsets[4] = y_g;
	offsets[5] = z_g;
}
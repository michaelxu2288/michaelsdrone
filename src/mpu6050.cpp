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


#define Read(r) i2c_smbus_read_byte_data(fd, r)
#define Write(r,v) i2c_smbus_write_byte_data(fd, r, v)

static int fd;

void mpu6050::init(){init(MPU6050_DEFAULT_ADDR);}

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
}

void mpu6050::wake_up(){
	Write(REG_PWR_MNG_1, Read(REG_PWR_MNG_1) | 0b01000000);
}

void mpu6050::sleep(){
	Write(REG_PWR_MNG_1, Read(REG_PWR_MNG_1) & (~0b01000000));
	
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
}

void mpu6050::set_clk(clk::clk set){
	Write(REG_PWR_MNG_1, Read(REG_PWR_MNG_1) & (~0b00000111) | set);

}

void mpu6050::set_dlpf_bandwidth(dlpf::dlpf set){
	Write(REG_CFG, Read(REG_CFG) & (~0b00000111) | set);
}

void mpu6050::set_fsync(fsync::fsync set){
	Write(REG_CFG, Read(REG_CFG) & (~0b00111000) | (set << 3));
}

void mpu6050::set_pwr_set(int set){

}

void mpu6050::read_raw(int * data){
	data[0] = Read(OUT_XACCL_H) << 8 | Read(OUT_XACCL_L);
	data[1] = Read(OUT_YACCL_H) << 8 | Read(OUT_YACCL_L);
	data[2] = Read(OUT_ZACCL_H) << 8 | Read(OUT_ZACCL_L);
	data[3] = Read(OUT_XGYRO_H) << 8 | Read(OUT_XGYRO_L);
	data[4] = Read(OUT_YGYRO_H) << 8 | Read(OUT_YGYRO_L);
	data[5] = Read(OUT_ZGYRO_H) << 8 | Read(OUT_ZGYRO_L);
}


void mpu6050::read(double * data){
	data[0] = ((double) (Read(OUT_XACCL_H) << 8 | Read(OUT_XACCL_L))) / accl_scale;
	data[1] = ((double) (Read(OUT_YACCL_H) << 8 | Read(OUT_YACCL_L))) / accl_scale;
	data[2] = ((double) (Read(OUT_ZACCL_H) << 8 | Read(OUT_ZACCL_L))) / accl_scale;
	data[3] = ((double) (Read(OUT_XGYRO_H) << 8 | Read(OUT_XGYRO_L))) / gyro_scale;
	data[4] = ((double) (Read(OUT_YGYRO_H) << 8 | Read(OUT_YGYRO_L))) / gyro_scale;
	data[5] = ((double) (Read(OUT_ZGYRO_H) << 8 | Read(OUT_ZGYRO_L))) / gyro_scale;
}

int mpu6050::query_register(int reg){
	return Read(reg);
}

void mpu6050::set_register(int reg, int data){
	Write(reg,data);
}

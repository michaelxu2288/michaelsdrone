#include <pca9685.h>

extern "C" {
	#include <linux/i2c-dev.h>
	#include <i2c/smbus.h>
}

#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <cstdlib>

#define read(r) i2c_smbus_read_byte_data(fd, r);
#define write(r,v) i2c_smbus_write_byte_data(fd, r, v);

static int fd = -1;

int pwm_regs[16][4] = {
    {0x06, 0x07, 0x08, 0x09},
    {0x0A, 0x0B, 0x0C, 0x0D},
    {0x0E, 0x0F, 0x10, 0x11},
    {0x02, 0x03, 0x14, 0x15},
    {0x16, 0x17, 0x18, 0x19},
    {0x1A, 0x1B, 0x1C, 0x1D},
    {0x1E, 0x1F, 0x20, 0x21},
    {0x22, 0x23, 0x24, 0x25},
    {0x26, 0x27, 0x28, 0x29},
    {0x2A, 0x2B, 0x2C, 0x2D},
    {0x2E, 0x2F, 0x30, 0x31},
    {0x32, 0x33, 0x34, 0x35},
    {0x36, 0x37, 0x38, 0x39},
    {0x3A, 0x3B, 0x3C, 0x3D},
    {0x3E, 0x3F, 0x40, 0x41},
    {0x42, 0x43, 0x04, 0x05},
};



void pca9685::init(){
    fd = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
	if (fd < 0) { //Catch errors
		std::cout << "ERR (pca9685.cpp:open()): Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config\n"; //Print error message
	}

	int status = ioctl(fd, I2C_SLAVE, PCA9685_ADDRESS); //Set the I2C bus to use the correct address
	if (status < 0) {
		std::cout << "ERR (pca9685.cpp:open()): Could not get I2C bus with " << PCA9685_ADDRESS << " address. Please confirm that this address is correct\n"; //Print error message
	}
}

void pca9685::destroy(){
    close(fd);
    sleep();
}

int pca9685::query_reg(int reg){
    return i2c_smbus_read_byte_data(fd, reg);
}

void pca9685::write_reg(int reg, int val){
    i2c_smbus_write_byte_data(fd, reg, val);
}

inline int round(double d){
    if(d < 0){
        return (((-d) - ((int)-d)) < 0.5 ? ((int) d) : ((int) d - 1));
    }
    return (d - ((int) d) < 0.5 ? (int) d : (int) d + 1);
}

static int freq = 200; // hz
static int per =  1000 / freq; // 5 ms
void pca9685::set_frequency(int frq){
// PRESCALE_VAL -> round(25000000 / (4096 * update_rate)) - 1
    freq = frq;
    per = 1000 / frq;
    int reg_val = round(25000000.0 / (4096 * freq)) - 1;

    write(PRESCALE, reg_val & 0xFF);
}
void pca9685::set_pwm_ms(int pwm, int ms){
    int cycle = 4096 * ms / per;
    write(pwm_regs[pwm][0], 0);
    write(pwm_regs[pwm][1], 0);
    write(pwm_regs[pwm][2], (cycle & 0xF00) >> 8);
    write(pwm_regs[pwm][3], cycle & 0xFF);
}
void pca9685::set_pwm_percent(int pwm, double percent){
    
}
void pca9685::set_pwm_on(int pwm, int on){
    
}
void pca9685::set_pwm_off(int pwm, int off){
    
}

void pca9685::wake_up(){
    write(MODE_1, 0b00000001);
}

void pca9685::sleep(){
    write(MODE_1, 0b00000001 | PCA9685_SLEEP);
}

void pca9685::restart(){
    
}
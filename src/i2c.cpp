#include <i2c.h>
#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

int i2c::get_device(int addr) {
    int fd = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
	if (fd < 0) { //Catch errors
		logger::err("Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config"); //Print error message
	}

	int status = ioctl(fd, I2C_SLAVE, addr); //Set the I2C bus to use the correct address
	if (status < 0) {
		logger::err("Could not get I2C bus with {} address. Please confirm that this address is correct", addr);
	}
    return fd;
}

int i2c::close_device(int fd) {
    return close(fd);
}

int i2c::read_byte(int fd, char reg) {
    char buf = reg;
    int n = read(fd, &buf, 1);
    return buf[0];
}

void i2c::read_burst(int fd, char reg, char * buf, int len) {
    buf[0] = reg;
    int n = read(fd, buf, 1);
}

void i2c::write_byte(int fd, char reg, int val) {
    char buf[2];
    buf[0] = reg;
    buf[1] = val;
    int n = write(fd, buf, 2);
}

void i2c::write_burst(int fd, char reg, char * buf, int len) {
    if(len > 20) {logger::err("Too many bytes to write: {}", len); return;}
    char writebuf[21];
    writebuf[0] = reg;
    while(len--){
        writebuf[len+1] = buf[len];
    }
    int n = write(fd, buf, len+1);
}

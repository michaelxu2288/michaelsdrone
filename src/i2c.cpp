#include <i2c.h>
#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <linux/types.h>

extern "C" {
	#include <linux/i2c-dev.h>
	#include <i2c/smbus.h>
}


// /* This is the structure as used in the I2C_SMBUS ioctl call */
// struct i2c_smbus_ioctl_data {
// 	__u8 read_write;
// 	__u8 command;
// 	__u32 size;
// 	union i2c_smbus_data __user *data;
// };

// /* This is the structure as used in the I2C_RDWR ioctl call */
// struct i2c_rdwr_ioctl_data {
// 	struct i2c_msg __user *msgs;	/* pointers to i2c_msgs */
// 	__u32 nmsgs;			/* number of i2c_msgs */
// };

// struct i2c_msg {
//         __u16 addr;        /* slave address                        */
//         __u16 flags;
// #define I2C_M_TEN                0x0010        /* this is a ten bit chip address */
// #define I2C_M_RD                0x0001        /* read data, from slave to master */
// #define I2C_M_NOSTART                0x4000        /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_REV_DIR_ADDR        0x2000        /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_IGNORE_NAK        0x1000        /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_NO_RD_ACK                0x0800        /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_RECV_LEN                0x0400        /* length will be first received byte */
//         __u16 len;                /* msg length                                */
//         __u8 *buf;                /* pointer to msg data                        */
// };

// int i2c::get_device(int addr) {
//     int fd = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
// 	if (fd < 0) { //Catch errors
// 		logger::err("Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config"); //Print error message
// 	}

// 	int status = ioctl(fd, I2C_SLAVE, addr); //Set the I2C bus to use the correct address
// 	if (status < 0) {
// 		logger::err("Could not get I2C bus with {} address. Please confirm that this address is correct", addr);
// 	}
//     return fd;
// }

// int i2c::close_device(int fd) {
//     return close(fd);
// }

// __u8 i2c::read_byte(int fd, __u8 reg) {
//     __u8 buf = reg;
//     int n = read(fd, &buf, 1);
//     if(n != 1){
//         perror("BRUH: ");
//     }
//     return buf;
// }

// void i2c::read_burst(int fd, __u8 reg, __u8 * buf, int len) {
//     buf[0] = reg;
//     int n = read(fd, buf, len);
//     if(n != len){
//         perror("COCK: ");
//     }
// }

// void i2c::write_byte(int fd, __u8 reg, __u8 val) {
//     __u8 buf[2];
//     buf[0] = reg;
//     buf[1] = val;
//     int n = write(fd, buf, 2);
//     if(n != 1){
//         perror("WRITE: ");
//     }
// }

// void i2c::write_burst(int fd, __u8 reg, __u8 * buf, int len) {
//     if(len > 20) {logger::err("Too many bytes to write: {}", len); return;}
//     __u8 writebuf[21];
//     writebuf[0] = reg;
//     while(len--){
//         writebuf[len+1] = buf[len];
//     }
//     int n = write(fd, buf, len+1);
//     if(n != len+1){
//         perror("WTF: ");
//     }
// }


i2c::device::device(int _addr) {
    addr = _addr;
    
    fd = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
	if (fd < 0) { //Catch errors
		logger::err("Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config"); //Print error message
        return;
	}

	int status = ioctl(fd, I2C_SLAVE, addr); //Set the I2C bus to use the correct address
	if (status < 0) {
		logger::err("Could not get I2C bus with {} address. Please confirm that this address is correct", addr);
	}
}

__u8 i2c::device::read_byte(__u8 reg) {
    i2c_rdwr_ioctl_data data;
    i2c_msg msgs[2];
    __u8 buf[1];
    buf[0] = reg;
    msgs[0].addr = addr;
    msgs[0].buf = buf;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    
    msgs[1].addr = addr;
    msgs[1].buf = buf;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = 1;

    data.msgs = msgs;
    data.nmsgs = 2;

    int n = ioctl(fd, I2C_RDWR, &data);
    if(n < 0) {
        perror("Error writing byte");
    }

    return buf[0];
}

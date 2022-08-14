#include <linux/types.h>

namespace i2c{

    // int get_device(int addr);
    // int close_device(int fd);

    // __u8 read_byte(int fd, __u8 reg);
    // void read_burst(int fd, __u8 reg, __u8 * buf, int len);

    // void write_byte(int fd, __u8 reg, __u8 val);
    // void write_burst(int fd, __u8 reg, __u8 * buf, int len);
    struct device {
        int fd;
        int addr;

        device(int addr);
        __u8 read_byte(__u8 reg);
        void read_burst(__u8 reg, __u8 * buf, int len);

        void write_byte(__u8 reg, __u8 val);
        void write_burst(__u8 reg, __u8 * buf, int len);
    };
}
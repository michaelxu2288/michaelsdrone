
namespace i2c{

    // int get_device(int addr);
    // int close_device(int fd);

    // char read_byte(int fd, char reg);
    // void read_burst(int fd, char reg, char * buf, int len);

    // void write_byte(int fd, char reg, char val);
    // void write_burst(int fd, char reg, char * buf, int len);
    struct device {
        int fd;
        int addr;

        device(int addr);
        char read_byte(char reg);
        void read_burst(char reg, char * buf, int len);

        void write_byte(char reg, char val);
        void write_burst(char reg, char * buf, int len);
    }
}

namespace i2c{

    int get_device(int addr);
    int close_device(int fd);

    int read_byte(int fd, int reg);
    void read_burst(int fd, int reg, int * buf, int len);

    void write_byte(int fd, int reg, int val);
    void write_burst(int fd, int reg, int * buf, int len);

}
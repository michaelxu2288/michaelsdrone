#include <logger.h>
#include <i2c.h>

int main() {
    int mpu = i2c::get_device(0x68);
    logger::info("0x{x}", i2c::read_byte(mpu, 0x75));
    
}
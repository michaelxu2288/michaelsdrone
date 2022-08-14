#include <logger.h>
#include <i2c.h>

int main() {
    // int mpu = i2c::get_device(0x68);
    i2c::device mpu(0x68);
    logger::info("{:x}", mpu.read_byte(0x75));

}
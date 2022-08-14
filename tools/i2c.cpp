#include <logger.h>
#include <i2c.h>

#include <cstdint>

int main() {
    // int mpu = i2c::get_device(0x68);
    i2c::device mpu(0x68);
    logger::info("WHOAMI 0x{:x}", mpu.read_byte(0x75));

    __u8 data[14];
    mpu.read_burst(0x3B, data, 14);
    for(int i = 0; i < 14; i += 2) {
        logger::info("0x{:2x} 0x{:2x} | {:d}", data[i], data[i+1], (((int16_t) (data[i] << 8)) | (data[i+1])));
    }

}
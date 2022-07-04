#include <bmp390.h>

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
#include <cmath>

static int fd = -1;

#define MOLAR_MASS_AIR 2.896e-2 // kg/mol
#define AVERAGE_SEA_LVL_PRESSURE 1.01325e5 // Pa
#define STANDARD_TEMP 288.15 // K
#define UNV_GAS_CONST 8.3143 // (N*m) / (mol * K)
#define GRAVITATIONAL_ACCELERATION 9.807 // m/s^2

void bmp390::init(){

	fd = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
	if (fd < 0) { //Catch errors
		std::cout << "ERR (bmp390.cpp:init()): Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config\n"; //Print error message
	}

	int status = ioctl(fd, I2C_SLAVE,BMP390_ADDR); //Set the I2C bus to use the correct address
	if (status < 0) {
		std::cout << "ERR (bmp390.cpp:init()): Could not get I2C bus with " << BMP390_ADDR << " address. Please confirm that this address is correct\n"; //Print error message
	}

    bmp390::soft_reset();
    i2c_smbus_write_byte_data(fd, REG_PWR_CTRL, BMP390_MODE);
    while((query_register(REG_PWR_CTRL) & 0b110000) == 0){
        i2c_smbus_write_byte_data(fd, REG_PWR_CTRL, BMP390_MODE);
        usleep(1000);
    }
    bmp390::acquire_calib_vars();
    //i2c_smbus_write_byte_data(fd, REG_CONFIG, BMP390_CONFIG);
    //i2c_smbus_write_byte_data(fd, REG_OSR, BMP390_OSRS);

}

double compensate_temp();
double compensate_pressure();
double compensate_pressure(double temp);

double par_t1, par_t2, par_t3;
double par_p1, par_p2, par_p3, par_p4, par_p5, par_p6, par_p7, par_p8, par_p9, par_p10, par_p11;

// int par_t1, par_t2, par_t3;
// int par_p1, par_p2, par_p3, par_p4, par_p5, par_p6, par_p7, par_p8, par_p9, par_p10, par_p11;
// int8_t par_t3, par_p3, par_p4, par_p7, par_p8, par_p10, par_p11;
// uint16_t par_t1, par_t2, par_p5, par_p6;
// int16_t par_p1, par_p2, par_p9;


int16_t combine(int byte1, int byte2) {
  // This code assumes that byte1 is in range, but allows for the possibility
  // that the values were originally in a signed char and so is now negative.
  return (int16_t) (((uint16_t) byte1 << 8) | byte2);
}

// void bmp390::acquire_calib_vars(){
//     int nvm_part_p11 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_P11);
//     int nvm_part_p10 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_P10);
//     int nvm_part_p9 = combine(i2c_smbus_read_byte_data(fd, NVM_PAR_P9_H),i2c_smbus_read_byte_data(fd, NVM_PAR_P9_L));
//     //nvm_part_p9 = ((nvm_part_p9 & 0x80) == 0x80 ? -(nvm_part_p9 & 0x7F) : nvm_part_p9 & 0x7F);
//     int nvm_part_p8 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_P8);
//     int nvm_part_p7 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_P7);
//     int nvm_part_p6 = (uint16_t) (i2c_smbus_read_byte_data(fd, NVM_PAR_P6_H)<<8)|i2c_smbus_read_byte_data(fd, NVM_PAR_P6_L);
//     int nvm_part_p5 = (uint16_t) (i2c_smbus_read_byte_data(fd, NVM_PAR_P5_H)<<8)|i2c_smbus_read_byte_data(fd, NVM_PAR_P5_L);
//     int nvm_part_p4 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_P4);
//     int nvm_part_p3 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_P3);
//     int nvm_part_p2 = combine(i2c_smbus_read_byte_data(fd, NVM_PAR_P2_H),i2c_smbus_read_byte_data(fd, NVM_PAR_P2_L));
//     //nvm_part_p2 = ((nvm_part_p2 & 0x80) == 0x80 ? -(nvm_part_p2 & 0x7F) : nvm_part_p2 & 0x7F);
//     int nvm_part_p1 = combine(i2c_smbus_read_byte_data(fd, NVM_PAR_P1_H),i2c_smbus_read_byte_data(fd, NVM_PAR_P1_L));
//     //nvm_part_p1 = ((nvm_part_p1 & 0x80) == 0x80 ? -(nvm_part_p1 & 0x7F) : nvm_part_p1 & 0x7F);
//     int nvm_part_t3 =  (int8_t) i2c_smbus_read_byte_data(fd, NVM_PAR_T3);
//     int nvm_part_t2 = (uint16_t) (i2c_smbus_read_byte_data(fd, NVM_PAR_T2_H)<<8)|i2c_smbus_read_byte_data(fd, NVM_PAR_T2_L);
//     int nvm_part_t1 = (uint16_t) (i2c_smbus_read_byte_data(fd, NVM_PAR_T1_H)<<8)|i2c_smbus_read_byte_data(fd, NVM_PAR_T1_L);

//     par_t1 = ((double) nvm_part_t1) * (256.0);
//     par_t2 = ((double) nvm_part_t2) / (1073741824.0);
//     par_t3 = ((double) nvm_part_t3) / (281474976710656.0);
    
//     par_p1 = (((double) nvm_part_p1) - 16384.0) / (137438953472.0);
//     par_p2 = (((double) nvm_part_p2) - 16384.0) / (536870912.0);
//     par_p3 = ((double) nvm_part_p3) / (4294967296.0);
//     par_p4 = ((double) nvm_part_p4) / (137438953472.0);
//     par_p5 = ((double) nvm_part_p5) * (8.0);
//     par_p6 = ((double) nvm_part_p6) / (64.0);
//     par_p7 = ((double) nvm_part_p7) / (256.0);
//     par_p8 = ((double) nvm_part_p8) / (32768.0);
//     par_p9 = ((double) nvm_part_p9) / (281474976710656.0);
//     par_p10 = ((double) nvm_part_p10) / (281474976710656.0);
//     par_p11 = ((double) nvm_part_p11) / (36893488147419103232.0);
// }


void bmp390::acquire_calib_vars(){

    #define BMP3_CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)

    
    uint16_t reg_par_t1;
    uint16_t reg_par_t2;
    int8_t reg_par_t3;
    int16_t reg_par_p1;
    int16_t reg_par_p2;
    int8_t reg_par_p3;
    int8_t reg_par_p4;
    uint16_t reg_par_p5;
    uint16_t reg_par_p6;
    int8_t reg_par_p7;
    int8_t reg_par_p8;
    int16_t reg_par_p9;
    int8_t reg_par_p10;
    int8_t reg_par_p11;

    uint8_t reg_data[20];
    for(int i = 0; i < 20; i ++){
        reg_data[i] = i2c_smbus_read_byte_data(fd, NVM_PAR_T1_L+i);
    }

    /* Temporary variable */
    double temp_var;

    /* 1 / 2^8 */
    temp_var = 0.00390625f;
    reg_par_t1 = BMP3_CONCAT_BYTES(reg_data[1], reg_data[0]);
    par_t1 = ((double)reg_par_t1 / temp_var);
    reg_par_t2 = BMP3_CONCAT_BYTES(reg_data[3], reg_data[2]);
    temp_var = 1073741824.0f;
    par_t2 = ((double)reg_par_t2 / temp_var);
    reg_par_t3 = (int8_t)reg_data[4];
    temp_var = 281474976710656.0f;
    par_t3 = ((double)reg_par_t3 / temp_var);
    reg_par_p1 = (int16_t)BMP3_CONCAT_BYTES(reg_data[6], reg_data[5]);
    temp_var = 1048576.0f;
    par_p1 = ((double)(reg_par_p1 - (16384)) / temp_var);
    reg_par_p2 = (int16_t)BMP3_CONCAT_BYTES(reg_data[8], reg_data[7]);
    temp_var = 536870912.0f;
    par_p2 = ((double)(reg_par_p2 - (16384)) / temp_var);
    reg_par_p3 = (int8_t)reg_data[9];
    temp_var = 4294967296.0f;
    par_p3 = ((double)reg_par_p3 / temp_var);
    reg_par_p4 = (int8_t)reg_data[10];
    temp_var = 137438953472.0f;
    par_p4 = ((double)reg_par_p4 / temp_var);
    reg_par_p5 = BMP3_CONCAT_BYTES(reg_data[12], reg_data[11]);

    /* 1 / 2^3 */
    temp_var = 0.125f;
    par_p5 = ((double)reg_par_p5 / temp_var);
    reg_par_p6 = BMP3_CONCAT_BYTES(reg_data[14], reg_data[13]);
    temp_var = 64.0f;
    par_p6 = ((double)reg_par_p6 / temp_var);
    reg_par_p7 = (int8_t)reg_data[15];
    temp_var = 256.0f;
    par_p7 = ((double)reg_par_p7 / temp_var);
    reg_par_p8 = (int8_t)reg_data[16];
    temp_var = 32768.0f;
    par_p8 = ((double)reg_par_p8 / temp_var);
    reg_par_p9 = (int16_t)BMP3_CONCAT_BYTES(reg_data[18], reg_data[17]);
    temp_var = 281474976710656.0f;
    par_p9 = ((double)reg_par_p9 / temp_var);
    reg_par_p10 = (int8_t)reg_data[19];
    temp_var = 281474976710656.0f;
    par_p10 = ((double)reg_par_p10 / temp_var);
    reg_par_p11 = (int8_t)reg_data[20];
    temp_var = 36893488147419103232.0f;
    par_p11 = ((double)reg_par_p11 / temp_var);
}


void bmp390::print_compensations(){
    printf("COMPUTED\n");
    printf("t1:  %10.f\n", par_t1);
    printf("t2:  %10.f\n", par_t2);
    printf("t3:  %10.f\n\n", par_t3);
    printf("p1:  %10.f\n", par_p1);
    printf("p2:  %10.f\n", par_p2);
    printf("p3:  %10.f\n", par_p3);
    printf("p4:  %10.f\n", par_p4);
    printf("p5:  %10.f\n", par_p5);
    printf("p6:  %10.f\n", par_p6);
    printf("p7:  %10.f\n", par_p7);
    printf("p8:  %10.f\n", par_p8);
    printf("p9:  %10.f\n", par_p9);
    printf("p10: %10.f\n", par_p10);
    printf("p11: %10.f\n\n", par_p11);
    printf("RAW \n");
    printf("t1:  %10d\n", ((query_register(NVM_PAR_T1_H) << 8) | query_register(NVM_PAR_T1_L)));
    printf("t2:  %10d\n", ((query_register(NVM_PAR_T2_H) << 8) | query_register(NVM_PAR_T2_L)));
    printf("t3:  %10d\n\n", query_register(NVM_PAR_T3));
    printf("p1:  %10d\n", ((query_register(NVM_PAR_P1_H) << 8) | query_register(NVM_PAR_P1_L)));
    printf("p2:  %10d\n", ((query_register(NVM_PAR_P2_H) << 8) | query_register(NVM_PAR_P2_L)));
    printf("p3:  %10d\n", query_register(NVM_PAR_P3));
    printf("p4:  %10d\n", query_register(NVM_PAR_P4));
    printf("p5:  %10d\n", ((query_register(NVM_PAR_P5_H) << 8) | query_register(NVM_PAR_P5_L)));
    printf("p6:  %10d\n", ((query_register(NVM_PAR_P6_H) << 8) | query_register(NVM_PAR_P6_L)));
    printf("p7:  %10d\n", query_register(NVM_PAR_P7));
    printf("p8:  %10d\n", query_register(NVM_PAR_P8));
    printf("p9:  %10d\n", ((query_register(NVM_PAR_P9_H) << 8) | query_register(NVM_PAR_P9_L)));
    printf("p10: %10d\n", query_register(NVM_PAR_P10));
    printf("p11: %10d\n\n", query_register(NVM_PAR_P11));
    
}





int bmp390::get_raw_press(){
    int high = (uint32_t) i2c_smbus_read_byte_data(fd, REG_PRESS_23_16);
    int low = ((uint32_t) i2c_smbus_read_byte_data(fd, REG_PRESS_15_8) << 8) | ((uint32_t) i2c_smbus_read_byte_data(fd, REG_PRESS_7_0));
    // std::cout << "PRESSURE: " << high << " / " << low << "\n";
    //Two's complement?

    return (high << 16) | low;
}

double bmp390::get_press(){
    return compensate_pressure();
    double raw_press = (float) get_raw_press();
    double temp = get_temp() * 100 * 16384 / 25;
    // std::cout << "temp: " << temp << "\n";
    // std::cout << "raw pressure: " << raw_press << "\n";
    double partial_data1;
    double partial_data2;
    double partial_data3;
    double partial_data4;
    double partial_out1;
    double partial_out2;

    partial_data1 = par_p6 * temp;
    partial_data2 = par_p7 * (temp * temp);
    partial_data3 = par_p8 * (temp * temp * temp);
    partial_out1 = par_p5 + partial_data1 + partial_data2 + partial_data3;
    // std::cout << "part out 1: " << partial_out1 << "\n";
    partial_data1 = par_p2 * temp;
    partial_data2 = par_p3 * (temp * temp);
    partial_data3 = par_p4 * (temp * temp * temp);
    partial_out2 = raw_press * (par_p1 + partial_data1 + partial_data2 + partial_data3);
    // std::cout << "part out 2: " << partial_out2 << "\n";

    partial_data1 = raw_press * raw_press;
    partial_data2 = par_p9 + par_p10 * temp;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + (raw_press * raw_press * raw_press) * par_p11;
    // std::cout << "part out 3: " << partial_data4 << "\n";

    return partial_out1 + partial_out2 + partial_data4;
}



double bmp390::get_press(double temp){
    return compensate_pressure(temp);
    double raw_press = (float) get_raw_press();
    temp *= 100;
    // temp *= 16384;
    // temp /= 25;
    // std::cout << "temp: " << temp << "\n";
    // std::cout << "raw pressure: " << raw_press << "\n";
    double partial_data1;
    double partial_data2;
    double partial_data3;
    double partial_data4;
    double partial_out1;
    double partial_out2;

    partial_data1 = par_p6 * temp;
    partial_data2 = par_p7 * (temp * temp);
    partial_data3 = par_p8 * (temp * temp * temp);
    partial_out1 = par_p5 + partial_data1 + partial_data2 + partial_data3;
    // std::cout << "part out 1: " << partial_out1 << "\n";
    partial_data1 = par_p2 * temp;
    partial_data2 = par_p3 * (temp * temp);
    partial_data3 = par_p4 * (temp * temp * temp);
    partial_out2 = raw_press * (par_p1 + partial_data1 + partial_data2 + partial_data3);
    // std::cout << "part out 2: " << partial_out2 << "\n";

    partial_data1 = raw_press * raw_press;
    partial_data2 = par_p9 + par_p10 * temp;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + (raw_press * raw_press * raw_press) * par_p11;
    // std::cout << "part out 3: " << partial_data4 << "\n";

    return partial_out1 + partial_out2 + partial_data4;
}


double height(double temp_c, double pressure_k){
    double temp_k = temp_c + 273.15;

    return - UNV_GAS_CONST * STANDARD_TEMP * log(pressure_k / AVERAGE_SEA_LVL_PRESSURE) / (MOLAR_MASS_AIR * GRAVITATIONAL_ACCELERATION);
}

double bmp390::get_height(){
    double temp_c = get_temp();
    double pressure_k = get_press(temp_c);
    return height(temp_c, pressure_k);
}

int bmp390::get_raw_temp(){
    int high = ((uint32_t) i2c_smbus_read_byte_data(fd, REG_TEMP_23_16));
    int low = ((uint32_t) i2c_smbus_read_byte_data(fd, REG_TEMP_15_8) << 8) | ((uint32_t) i2c_smbus_read_byte_data(fd, REG_TEMP_7_0));
    // std::cout << "TEMPERATURE: " << high << " / " << low << "\n";
    return (high << 16) | low;
}




void bmp390::get_data(double * data){
    data[0] = bmp390::get_temp();
    data[1] = bmp390::get_press(data[0]);
    data[2] = height(data[0], data[1]);
}

double bmp390::get_temp(){
    return compensate_temp();

    double raw_temp = get_raw_temp();
    double pd1 = raw_temp - par_t1;

    return par_t2 * pd1 + par_t3 * (pd1 * pd1);
}

















double compensate_temp(){

    uint32_t uncomp_temp = bmp390::get_raw_temp();
    double partial_data1;
    double partial_data2;

    partial_data1 = (double)(uncomp_temp - par_t1);
    partial_data2 = (double)(partial_data1 * par_t2);

    /* Update the compensated temperature in calib structure since this is
     * needed for pressure calculation */
    return partial_data2 + (partial_data1 * partial_data1) *
                                             par_t3;

    /* Returns compensated temperature */
    // return calib_data->quantized_calib_data.t_lin;
}


double pow_bmp3(double base, uint8_t power)
{
    double pow_output = 1;

    while (power != 0)
    {
        pow_output = (double) base * pow_output;
        power--;
    }

    return pow_output;
}
double compensate_pressure()
{
    
    double t_lin = compensate_temp();
    int uncomp_pressure = bmp390::get_raw_press();
    /* Variable to store the compensated pressure */
    double comp_press;

    /* Temporary variables used for compensation */
    double partial_data1;
    double partial_data2;
    double partial_data3;
    double partial_data4;
    double partial_out1;
    double partial_out2;

    partial_data1 = par_p6 * t_lin;
    partial_data2 = par_p7 * pow_bmp3(t_lin, 2);
    partial_data3 = par_p8 * pow_bmp3(t_lin, 3);
    partial_out1 = par_p5 + partial_data1 + partial_data2 + partial_data3;
    partial_data1 = par_p2 * t_lin;
    partial_data2 = par_p3 * pow_bmp3(t_lin, 2);
    partial_data3 = par_p4 * pow_bmp3(t_lin, 3);
    partial_out2 = uncomp_pressure *
                   (par_p1 + partial_data1 + partial_data2 + partial_data3);
    partial_data1 = pow_bmp3((double)uncomp_pressure, 2);
    partial_data2 = par_p9 + par_p10 * t_lin;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + pow_bmp3((double)uncomp_pressure, 3) * par_p11;
    comp_press = partial_out1 + partial_out2 + partial_data4;

    return comp_press;
}

double compensate_pressure(double temp)
{
    
    double t_lin = temp;
    int uncomp_pressure = bmp390::get_raw_press();
    /* Variable to store the compensated pressure */
    double comp_press;

    /* Temporary variables used for compensation */
    double partial_data1;
    double partial_data2;
    double partial_data3;
    double partial_data4;
    double partial_out1;
    double partial_out2;

    partial_data1 = par_p6 * t_lin;
    partial_data2 = par_p7 * pow_bmp3(t_lin, 2);
    partial_data3 = par_p8 * pow_bmp3(t_lin, 3);
    partial_out1 = par_p5 + partial_data1 + partial_data2 + partial_data3;
    partial_data1 = par_p2 * t_lin;
    partial_data2 = par_p3 * pow_bmp3(t_lin, 2);
    partial_data3 = par_p4 * pow_bmp3(t_lin, 3);
    partial_out2 = uncomp_pressure *
                   (par_p1 + partial_data1 + partial_data2 + partial_data3);
    partial_data1 = pow_bmp3((double)uncomp_pressure, 2);
    partial_data2 = par_p9 + par_p10 * t_lin;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + pow_bmp3((double)uncomp_pressure, 3) * par_p11;
    comp_press = partial_out1 + partial_out2 + partial_data4;

    return comp_press;
}

























void bmp390::stop(){
    close(fd);
}

int bmp390::query_register(int reg){
    return i2c_smbus_read_byte_data(fd, reg);
}

void bmp390::soft_reset(){
    // std::cout << "FUCK SHIT CUNT DICK\n";
    i2c_smbus_write_byte_data(fd, REG_CMD, BMP390_SOFT_RESET);
}






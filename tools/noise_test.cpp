#include <bmp390.h>
#include <mpu6050.h>
#include <logger.h>
#include <timer.h>
#include <unistd.h>

timer t;

#define N 3000
int i = 0;

double mpu6050_data[N][6]; 
double bmp390_data[N][3];

double mpu6050_avg[6] = {0,0,0,0,0,0};
double bmp390_avg[3] = {0,0,0};

void loop() {
    if(i >= N) {
        t.running = false;
        return;
    }

    bmp390::get_data(bmp390_data[i]);
    mpu6050::read(mpu6050_data[i]);

    
    for(int j = 0; j < 6; j ++) {
        mpu6050_data[i][j] *= 9.81;
        mpu6050_avg[j] += mpu6050_data[i][j];
    }

    for(int j = 0; j < 3; j ++) {
        bmp390_avg[j] += bmp390_data[i][j];
    }

    // logger::info("{:10.7f} {:10.7f} {:10.7f} {:10.7f} {:10.7f} {:10.7f} {:10.7f} {:10.7f} {:10.7f}", mpu6050_data[i][0], mpu6050_data[i][1], mpu6050_data[i][2], mpu6050_data[i][3], mpu6050_data[i][4], mpu6050_data[i][5], bmp390_data[i][0], bmp390_data[i][1], bmp390_data[i][2]);

    i++;
}

int main() {
    
    {
        logger::info("Initializing MPU6050.");
        mpu6050::init();
        mpu6050::set_accl_set(mpu6050::accl_range::g_2);
        mpu6050::set_gyro_set(mpu6050::gyro_range::deg_250);
        mpu6050::set_clk(mpu6050::clk::y_gyro);
        mpu6050::set_fsync(mpu6050::fsync::input_dis);
        mpu6050::set_dlpf_bandwidth(mpu6050::dlpf::hz_5);
        mpu6050::wake_up();
        mpu6050::calibrate(7);
        logger::info("Finished intializing the MPU6050.");
    }
    {
        logger::info("Initializing BMP390.");
        bmp390::init();
        bmp390::soft_reset();
        bmp390::set_oversample(bmp390::oversampling::STANDARD, bmp390::ULTRA_LOW_POWER);
        bmp390::set_iir_filter(bmp390::COEFF_3);
        bmp390::set_output_data_rate(bmp390::hz50);
        bmp390::set_enable(true, true);
        
        bmp390::set_enable_fifo(true, true);
        bmp390::set_fifo_stop_on_full(false);

        bmp390::set_pwr_mode(bmp390::NORMAL);
        usleep(100000);
        bmp390::flush_fifo();
        logger:info("Finished initializing the BMP390.");
    }

    t.start(loop, 1000/50);
    while(t.running) {
        usleep(10000);
    }



    for(int j = 0; j < 6; j ++) {
        mpu6050_avg[j] /= N;
    }

    for(int j = 0; j < 3; j ++) {
        bmp390_avg[j] /= N;
    }

    double mpu6050_var[6] = {0,0,0,0,0,0};
    double bmp390_var[3] = {0,0,0};

    for(int k = 0; k < N; k ++) {
        double a;
        for(int j = 0; j < 6; j ++) {
            a = mpu6050_data[k][j] - mpu6050_avg[j];
            mpu6050_var[j] += a * a;
        }
        for(int j = 0; j < 3; j ++) {
            a = bmp390_data[k][j] - bmp390_avg[j];
            bmp390_var[j] += a * a;
        }
    }   
    for(int j = 0; j < 6; j ++) {
        mpu6050_var[j] /= N;
    }

    for(int j = 0; j < 3; j ++) {
        bmp390_var[j] /= N;
    }

    logger::info("Results: MPU6050 \n\tax={:10.7f}±{:10.7f}\n\tay={:10.7f}±{:10.7f}\n\taz={:10.7f}±{:10.7f}\n\tgx={:10.7f}±{:10.7f}\n\tgy={:10.7f}±{:10.7f}\n\tgz={:10.7f}±{:10.7f}", mpu6050_avg[0], mpu6050_var[0], mpu6050_avg[1], mpu6050_var[1], mpu6050_avg[2], mpu6050_var[2], mpu6050_avg[3], mpu6050_var[3], mpu6050_avg[4], mpu6050_var[4], mpu6050_avg[5], mpu6050_var[5]);
    logger::info("Results: BMP390 \n\t{:10s}={:10.7f}±{:10.7f}\n\t{:10s}={:10.7f}±{:10.7f}\n\t{:10s}={:10.7f}±{:10.7f}", "temp", bmp390_avg[0], bmp390_var[0], "pressure", bmp390_avg[1], bmp390_var[1], "altitude", bmp390_avg[2], bmp390_var[2]);
}
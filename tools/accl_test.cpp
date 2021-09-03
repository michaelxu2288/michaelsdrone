#include <MPU6050.h>
#include <unistd.h>
#include <iostream>

int main(){
    MPU6050 bruh(0x68, false);
    setvbuf(stdout, NULL,_IONBF,0);
    float x,y,z;
    int i = 30;
    while(i--){
        bruh.getAccel(&x, &y,&z);
        std::cout<<x << " " << y << " " << z;
        usleep(100000);
    }

    
}
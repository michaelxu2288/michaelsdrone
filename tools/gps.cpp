#include <logger.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("/dev/serial0", O_RDWR);
    
    char buf[200];



    while(true) {
        int len = read(fd, buf, 200);
        // while(len --){
        //     buf[len] = ~buf[len];
        // }
        logger::info("length: {}, data: {}", len, buf);
    }

}
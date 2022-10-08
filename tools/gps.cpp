#include <logger.h>
#include <unistd.h>
#include <fcntl.h>

#define INTERNAL_BUFFER 1024

int main() {
    int fd = open("/dev/serial0", O_RDWR);
    
    char buf[INTERNAL_BUFFER];


    while(true) {
        int len = read(fd, buf, INTERNAL_BUFFER);
        // while(len --){
        //     buf[len] = ~buf[len];
        // }
        logger::info("length: {}, data: {}", len, buf);
    }

}
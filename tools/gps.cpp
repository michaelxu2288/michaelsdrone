#include <logger.h>
#include <unistd.h>
#include <fcntl.h>

#define INTERNAL_BUFFER 1024

int main() {
    int fd = open("/dev/serial0", O_RDWR);
    
    char buf[INTERNAL_BUFFER];
    int len = 0;

    while(true) {
        int chg_len = read(fd, buf + len, INTERNAL_BUFFER - len);
        if(chg_len != 0) {
            len += chg_len;
            logger::info("length: {}, data: {}", len, buf);
        }
    }

}
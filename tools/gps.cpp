#include <logger.h>
#include <unistd.h>
#include <fcntl.h>

#define INTERNAL_BUFFER 1024

int main() {
    int fd = open("/dev/serial0", O_RDWR | O_BLOCK);
    
    char buf[INTERNAL_BUFFER];
    int len = 0;

    while(true) {
        int chg_len += read(fd, buf + chg_len, INTERNAL_BUFFER);
        if(chg_len != 0) {
            len += chg_len;
            logger::info("length: {}, data: {}", len, buf);
        }
    }

}
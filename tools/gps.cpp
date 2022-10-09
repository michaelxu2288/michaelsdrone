#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <util.h>

#define INTERNAL_BUFFER 1024

int main() {
    int fd = open("/dev/serial0", O_RDWR);
    
    char buf[INTERNAL_BUFFER];
    char buf2[INTERNAL_BUFFER];
    int len = 0;

    while(true) {
        int chg_len = read(fd, buf + len, INTERNAL_BUFFER - len);
        if(chg_len != 0) {
            len += chg_len;
            buf[len] = '\0';
            int i = cstr::index_of(buf, "\r\n");
            // if(i >= 0) {
            //     logger::info("i: {}, data: {}", i, buf);
            //     cstr::shift(buf, i+2, buf2);
            //     len -= i+2;
            //     logger::info("length: {}, data: {}", i+2, buf2);
            // }

            logger::info("{}", buf);

        }
    }

}
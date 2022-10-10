#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <util.h>
#include <string>
#include <vector>

#define INTERNAL_BUFFER 1024


void process_nmea_line(char * line) {
    
}

int main() {
    int fd = open("/dev/serial0", O_RDWR);
    
    char buf[INTERNAL_BUFFER];
    std::string current = "";


    int len = 0;

    while(true) {
        int chg_len = read(fd, buf, INTERNAL_BUFFER);
        if(chg_len != 0) {
            current += buf;
            auto i = current.find("\r\n");
            while(i != std::string::npos) {
                // logger::info("i: {}, data: {}", i, buf);
                std::string msg = current.substr(0, i);
                current = current.substr(i+2);
                // logger::info("length: {}, data: {}", i, s);
                
                std::vector<std::string> args;
                str::split(msg, ",", args);
                for(int i = 0; i < args.size(); i ++) {
                    logger::info("arg: {}", args[i]);
                }

                i = current.find("\r\n");
            }
        }
    }

}
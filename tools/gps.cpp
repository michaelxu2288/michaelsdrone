#include <logger.h>
#include <unistd.h>
#include <fcntl.h>
#include <util.h>
#include <string>
#include <vector>

#define INTERNAL_BUFFER 1024

enum gps_talkers{
    gps = 0,
    glonass = 1,
    galileo = 2,
    beidou = 3
};
char * talkers_to_string[4] = {"gps", "glonass", "galileo" , "beidou"};




void process_nmea_line(const std::vector<std::string> & nmea_args, const std::string & nmea_str="") {
    if(nmea_args.size() <= 1 || nmea_args[0].length() != 6 || nmea_args[0][0] != '$') return;

    gps_talkers talker;
    logger::warn("NMEA cmd: {}", nmea_args[0]);

    if(nmea_args[0][1] == 'G') {
        switch(nmea_args[0][2]) {
        case 'B': // beidou
            talker = beidou;
            break;
        case 'A': // Galileo
            talker = galileo;
            break;
        case 'P': // Navstar GPS
            talker = gps;
            break;
        case 'L': // Glonass
            talker = glonass;
            break;
        default:
            break;
        }
    }else if(nmea_args[0][1] == 'B' && nmea_args[0][2] == 'D') {
        // Beidou
    }else {
        // Unknown
        logger::warn("Unknown Constellation \"{}\" : \"{}\"", nmea_args[0].substr(1,2), nmea_str);
        return;
    }
    
    if(nmea_args[0].rfind("GGA", 3)) {
        // Global Positioning System Fixed Data 
    }else if(nmea_args[0].rfind("GLL", 3)) {
        // Geographic Position-- Latitude and Longitude 
    }else if(nmea_args[0].rfind("GSA", 3)) {
        // GNSS DOP and active satellites 
    }else if(nmea_args[0].rfind("GSV", 3)) {
        // GNSS satellites in view 
    }else if(nmea_args[0].rfind("RMC", 3)) {
        // Recommended minimum specific GPS data
    }else if(nmea_args[0].rfind("VTG", 3)) {
        // Course over ground and ground speed 
    }else {
        // Unknown
        logger::warn("Unknown Sentence \"{}\" : \"{}\"", nmea_args[0].substr(3), nmea_str);
        return;
    }

    logger::info("Constellation: {} : {}", talkers_to_string[talker], nmea_args[0]);
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
                std::string nmea_msg = current.substr(0, i);
                current = current.substr(i+2);
                // logger::info("length: {}, data: {}", i, s);
                
                std::vector<std::string> nmea_args;
                str::split(nmea_msg, ",", nmea_args);
                
                process_nmea_line(nmea_args, nmea_msg);
                    
                // for(int i = 0; i < args.size(); i ++) {
                //     logger::info("arg: {}", args[i]);
                // }

                i = current.find("\r\n");
            }
        }
    }

}
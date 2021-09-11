#ifndef LOGGER_H
#define LOGGER_H

namespace logger {

    enum level {
        debug, info, critical, warn, error
    };

    void set_log_file(char * filename);

    void log(level lvl, char * msg);


    void debug(char * msg) {log(level::debug, msg);}
    void info(char * msg) {log(level::info, msg);}
    void crit(char * msg) {log(level::critical, msg);}
    void warn(char * msg) {log(level::warn, msg);}
    void err(char * msg) {log(level::error, msg);}

};

#endif
#ifndef LOGGER_H
#define LOGGER_H

namespace logger {

    enum level {
        DEBUG, INFO, CRITICAL, WARN, ERROR
    };

    void set_log_file(char * filename);

    void log(level lvl, char * msg);


    void debug(char * msg);
    void info(char * msg);
    void crit(char * msg);
    void warn(char * msg);
    void err(char * msg);

};

#endif
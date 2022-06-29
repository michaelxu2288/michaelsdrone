#ifndef LOGGER_H
#define LOGGER_H

namespace logger {

    enum level {
        DEBUG=0, INFO=1, CRITICAL=2, WARN=3, ERROR=4
    };

    void set_log_file(const char * filename);

    void log(level lvl, const char * msg);


    void debug(const char * msg);
    void info(const char * msg);
    void crit(const char * msg);
    void warn(const char * msg);
    void err(const char * msg);

};

#endif
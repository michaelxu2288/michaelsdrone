#ifndef REPORTER_H
#define REPORTER_H

// #include <string_view>

namespace reporter {
    // void set_connection()

    void bind_str(const char * name, char * str);
    void bind_bool(const char * name, bool * value);
    void bind_int(const char * name, int * value);
    void bind_dbl(const char * name, double * value);

    void get_report(char * report);
};





#endif
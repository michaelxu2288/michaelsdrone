#ifndef PARAMETERS_H_GUARD
#define PARAMETERS_H_GUARD
#include <string>
#include <functional>
#include <vector>
namespace parameters {

    void bind_str(const char * name, std::string * str, bool readonly = true);
    void bind_bool(const char * name, bool * value, bool readonly = true);
    void bind_int(const char * name, int * value, bool readonly = true);
    void bind_dbl(const char * name, double * value, bool readonly = true);

    void bind_axis(int axis, double * value);

    void post_bind_setup();

    void init_gamepad();
    void init_gamepad_thread();

    


    // void bind_button(int button, std::function<> );

    std::string get_report();
    std::string get_keys();
    std::string get_json_report();

    void chg(const char * cmd);
    void chg(std::vector<std::string> & args, int i);
}

#endif
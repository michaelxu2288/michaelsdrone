#ifndef CONFIG_INCLUDE_GUARD
#define CONFIG_INCLUDE_GUARD
#include <string>
#include <json.h>
namespace config{

    // struct config {
    //     json::JSON base;

    //     int get_config_int(const char * name, int dft=0);
    //     double get_config_dbl(const char * name, double dft=0);
    //     std::string get_config_str(const char * name, std::string dft = "");
    // };

    void load_file();
    void write_to_file();

    void load_file(const char * filename);
    void write_to_file(const char * filename);

    int get_config_int(const char * name, int dft=0);
    double get_config_dbl(const char * name, double dft=0   );
    std::string get_config_str(const char * name, std::string dft = "");

    void set_config(const char * name, int value);
    void set_config(const char * name, double value);
}




#endif
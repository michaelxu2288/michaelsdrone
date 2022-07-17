#ifndef CONFIG_INCLUDE_GUARD
#define CONFIG_INCLUDE_GUARD
#include <string>
// #include <json.h>
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

    /**
     * @brief Get the config int 
     * 
     * @param name Name of the config int "a.b.c => a:{b:{c:0}}}"
     * @param dft Default value if config variable isnt found
     * @return int 
     */
    int get_config_int(const char * name, int dft=0);
    double get_config_dbl(const char * name, double dft=0   );
    std::string get_config_str(const char * name, std::string dft = "");
    bool get_config_bool(const char * name, bool dft);

    void set_config(const char * name, int value);
    void set_config(const char * name, double value);
}




#endif
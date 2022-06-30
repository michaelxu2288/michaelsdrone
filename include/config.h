#ifndef CONFIG_INCLUDE_GUARD
#define CONFIG_INCLUDE_GUARD

namespace config{
    void load_file(const char * filename);
    void write_to_file(const char * filename);

    int get_config_int(const char * name, int default=0);
    double get_config_dbl(const char * name, double default=0   );

    void set_config(const char * name, int value);
    void set_config(const char * name, double value);

}




#endif
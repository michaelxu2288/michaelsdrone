#ifndef CONFIG_INCLUDE_GUARD
#define CONFIG_INCLUDE_GUARD


#include <rapidjson/rapidjson.h>


namespace config{
    
    class config {
        // double
        
    }

    void load_file(const char * filename);
    void write_to_file(const char * filename);

    void get_config(config& configuration);

}




#endif

#include <config.h>
// #include <rapidjson/document.h>
#include <json.h>
#include <fstream>
#include <string>
#include <logger.h>


static json::JSON configuration;
// static char * filename;
void config::load_file(const char * filename){
    std::ifstream in(filename);
    if(in){
        std::string str;
        std::getline(in, str, '\0');
        
        configuration = json::JSON::Load(str);
    }
}

void config::write_to_file(const char * filename){
    std::ofstream out(filename);
    if(out){
        out << configuration;
        out.close();
    }else{
        logger::warn("Couldn't create configuration file\n.");
    }
}

int config::get_config_int(const char * name, int dft){
    if(configuration.hasKey(name)){
        return configuration[name].ToInt();
    }
    configuration[name]=dft;
    return dft;
}

double config::get_config_dbl(const char * name, double dft){
    if(configuration.hasKey(name)){
        return configuration[name].ToFloat();
    }
    configuration[name]=dft;
    return dft;
}

void config::set_config(const char * name, int value){
    configuration[name]=value;
}

void config::set_config(const char * name, double value){
    configuration[name]=value;
}

#include <config.h>
// #include <rapidjson/document.h>
#include <json.h>
#include <fstream>
#include <string>
#include <logger.h>
#include <settings.h>

static json::JSON configuration;
static bool loaded = false;
// static char * filename;
void config::load_file(const char * filename){
    std::ifstream in(filename);
    if(in){
        std::string str;
        std::getline(in, str, '\0');
        
        configuration = json::JSON::Load(str);
        loaded = true;
    }
}

void config::write_to_file(const char * filename){
    if(!loaded){
        logger::warn("You didnt load the config file. This program will not override the config file.");
    }
    std::ofstream out(filename);    
    if(out){
        loaded = true;
        out << configuration;
        out.close();
    }else{
        logger::warn("Couldn't create configuration file\n.");
    }
}

void config::load_file(){
    if(!loaded){
        config::load_file(DEFAULT_CONFIG_FILE);
    }
}

void config::write_to_file(){
    config::write_to_file(DEFAULT_CONFIG_FILE);
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

std::string config::get_config_str(const char * name, std::string dft){
    if(configuration.hasKey(name)){
        return configuration[name].ToString();
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
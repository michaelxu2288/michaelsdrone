
#include <config.h>
// #include <rapidjson/document.h>
#include <json.h>
#include <fstream>
#include <string>
#include <logger.h>
#include <settings.h>
#include <mutex>

static json::JSON configuration;
static bool loaded = false;

std::mutex config_mutex;

// static char * filename;
void config::load_file(const char * filename){
    std::lock_guard<std::mutex> config_lock(config_mutex);
    std::ifstream in(filename);
    if(in){
        std::string str;
        std::getline(in, str, '\0');
        
        configuration = json::JSON::Load(str);
    }
}

void config::write_to_file(const char * filename){
    std::lock_guard<std::mutex> config_lock(config_mutex);

    std::ofstream out(filename);    
    if(out){
        logger::info("Saving configuration to \"{}\".", filename);
        out << configuration;
        out.close();
    }else{
        logger::warn("Couldn't create configuration file\n.");
    }
}

void config::load_file(){
    config::load_file(DEFAULT_CONFIG_FILE);
}

void config::write_to_file(){
    config::write_to_file(DEFAULT_CONFIG_FILE);
}

int config::get_config_int(const char * name, int dft){
    std::lock_guard<std::mutex> config_lock(config_mutex);
    if(configuration.hasKey(name)){
        return configuration[name].ToInt();
        
    }
    configuration[name]=dft;
    return dft;
}

double config::get_config_dbl(const char * name, double dft){
    std::lock_guard<std::mutex> config_lock(config_mutex);
    if(configuration.hasKey(name)){
        return configuration[name].ToFloat();
    }
    configuration[name]=dft;
    return dft;
}

std::string config::get_config_str(const char * name, std::string dft){
    std::lock_guard<std::mutex> config_lock(config_mutex);
    if(configuration.hasKey(name)){
        return configuration[name].ToString();
    }
    configuration[name]=dft;
    return dft;
}

void config::set_config(const char * name, int value){
    std::lock_guard<std::mutex> config_lock(config_mutex);
    configuration[name]=value;
}

void config::set_config(const char * name, double value){
    std::lock_guard<std::mutex> config_lock(config_mutex);
    configuration[name]=value;
}
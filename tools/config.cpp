#include <config.h>
#include <logger.h>
#include <json.hpp>

int main(){
    // json::JSON s = json::Object();
    // s["bruh"] = json::Object();
    // logger::info("{}", s.dump());
    // s = s["bruh"];
    // logger::info("{}", s.dump());
    config::load_file("config/test.json");
    config::get_config_int("a.b.c.d", 1);
    config::write_to_file("config/test.json");
}
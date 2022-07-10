// #include <config.h>
#include <logger.h>
#include <json.hpp>

int main(){
    nlohmann::json s = nlohmann::json::object(); // json::Object();
    s["bruh"] = nlohmann::json::object();
    logger::info("{}", s.dump());
    s = s["bruh"];
    logger::info("{}", s.dump());

    // config::load_file("config/test.json");
    // config::get_config_int("a.b.c.d", 1);
    // config::write_to_file("config/test.json");
}
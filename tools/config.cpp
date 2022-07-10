#include <config.h>
#include <logger.h>
#include <json.hpp>

int main(){
    // nlohmann::json s = nlohmann::json::object(); // json::Object();
    // nlohmann::json * k = &s["a"];
    // k = &((*k)["b"]);
    // k = &((*k)["c"]);
    // k = &((*k)["d"]);
    // *k = 0;
    // // k["a"]["b"]["c"]["d"] = 0;
    // // s["bruh"] = nlohmann::json::object();
    // logger::info("{}", s.dump());
    // s = s["bruh"];
    // logger::info("{}", s.dump());
    // logger::info("{}", k->dump());

    config::load_file("config/test.json");
    logger::info("{}", config::get_config_int("a.b.c.d", 1));
    logger::info("{}", config::get_config_dbl("a.b.c.d", 1));
    config::write_to_file("config/test.json");
}
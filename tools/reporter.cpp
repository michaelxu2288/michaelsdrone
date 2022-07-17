#include <logger.h>
#include <reporter.h>
#include <string>

int main(){
    bool a = false;
    int b = 0;
    double c = 0.0;
    std::string str = "Hello World!";
    reporter::bind_bool("a", &a);
    reporter::bind_int("b", &b);
    reporter::bind_dbl("c", &c);
    reporter::bind_str("str", &str);
    logger::info("keys: {}", reporter::get_keys());
    logger::info("vals: {}", reporter::get_report());
    a = true;
    b = 1321312;
    c = 3.141592;
    str = "Goodbye!";

    logger::info("vals: {}", reporter::get_report());
}
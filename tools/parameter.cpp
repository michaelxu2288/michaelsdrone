#include <parameters.h>
#include <logger.h>

int main(){
    bool a = false;
    int b = 0;
    double c = 0.0;
    std::string str = "Hello World!";
    bool aw = false;
    int bw = 0;
    double cw = 0.0;
    std::string strw = "bruh moment.";

    parameters::bind_bool("a", &a, false);
    parameters::bind_int("b", &b, false);
    parameters::bind_dbl("c", &c, false);
    parameters::bind_str("str", &str, false);
    
    parameters::bind_bool("a_w", &aw, true);
    parameters::bind_int("b_w", &bw, true);
    parameters::bind_dbl("c_w", &cw, true);
    parameters::bind_str("str_w", &strw, true);

    // parameters::post_bind_setup();

    logger::info("json: {}", parameters::get_json_report());
    a = true;
    b = 1321312;
    c = 3.141592;
    str = "Goodbye!";

    aw = true;
    bw = 876567898;
    cw = 2.718281828;
    strw = "bruv";

    logger::info("json: {}", parameters::get_json_report());
    
    parameters::chg("0 1 0");
    parameters::chg("0 2 -1.14");

    logger::info("json: {}", parameters::get_json_report());
}
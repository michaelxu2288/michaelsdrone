#include <cstdio>
#include <time.h>
#include <logger.h>
#include <time.h>

void logger::set_log_file(char * filename){

}

static char buf100[100];
static char buf20A[20];
static char buf20B[20];

static char ** titles = {"Debug", "Info", "CRITICAL", "WARNING", "ERROR"};

void logger::debug(char * msg) {
    log(level::debug, msg);
}
void logger::info(char * msg) {
    log(level::info, msg);
}
void logger::crit(char * msg) {
    log(level::critical, msg);
}
void logger::warn(char * msg) {
    log(level::warn, msg);
   }
void logger::err(char * msg) {
    log(level::error, msg);
}

void logger::log(level lvl, char * msg){
    
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    strftime(buf20A, "%c", timeinfo);
    sprintf(buf100, "[%s %s] %s", titles[lvl], buf20A, msg);
    puts(buf100);
}
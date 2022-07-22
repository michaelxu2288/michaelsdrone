#include <iostream>
#include <logger.h>
#include <unistd.h>


int main(){
    char cwd[1000];
    if(getcwd(cwd, 1000) != NULL){
        logger::info("Current working directory: {}", cwd);
    }else {
        logger::err("Error with getcwd()");
        // perror("Error with getcwd()")
    }
}
#include <iostream>
#include <unistd.h>
#include "process_control.h"
#include "arg_parse.h"

void process(const std::string &arg){

    if(arg == "exit"){
        return;
    }
    int rc = fork();

    if (rc < 0){
        std::cerr << "Forking error" << std::endl;
        return;
    }
    else if(rc == 0){ // Child
        std::vector<std::string> args = parseArgs(arg); // What is a const vector? and what does refernece do again
        
        // Convert args to char *array
        // TODO: const char * vs char * const???
        char **exec = new char*[args.size()+1];

        for(int i = 0; i < args.size(); i++){
            exec[i] = &args[i][0];
        }
        exec[args.size()+1] = nullptr;
        // call execv with these args
        execv(exec[0], exec);

        std::cerr << "execv error" << std::endl; // execv only returns if an error occured
        return;
    }
    else{ // Parent
        int rc_wait = wait(NULL);
    }

    return;
}


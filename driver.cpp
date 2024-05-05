#include <iostream>
#include "process_manager.h"
#include "job_control.h"
    
    /* TODO:
        Change "arg" in argument list to "command"
        Singleton refactor. How to inherit?
        Signal handling
        Posix_spawn
    */


int main(){

    std::string input;
    ProcessManager command = ProcessManager();
    //JobControl control = JobControl();
    //control.init_parallel();

    do{
        std::cout << "shell> ";
        std::getline(std::cin, input);

        command.process_command(input);

    }while(input != "exit");

}
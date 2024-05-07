#include <iostream>
#include "process_manager.h"
#include "job_control.h"
    
    /* TODO:
        Singleton refactor. How to inherit?
    */


int main(){

    std::string input;
    ProcessManager command = ProcessManager();
    JobControl::init_parallel();

    do{
        std::cout << "shell> ";
        std::getline(std::cin, input);

        command.process_command(input);

    }while(input != "exit");

}

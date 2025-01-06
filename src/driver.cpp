#include <iostream>
#include "../include/process_manager.h"
#include "../include/job_control.h"


int main(){

    std::string input;
    ProcessManager command;
    JobControl::init_parallel();

    do{
        std::cout << "shell> ";
        std::getline(std::cin, input);

        command.process_command(input);

    }while(input != "exit");

}

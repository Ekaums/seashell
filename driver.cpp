#include <iostream>
#include "process_manager.h"
#include "job_control.h"

int main(){
    
    /* TODO: 
        Namespace refactor. How to inherit?
        Running parallel commands with piping
        Signal handling
    */


    std::string input;
    ProcessManager command = ProcessManager();
    // JobControl control = JobControl();
    // control.init_parallel();

    do{
        std::cout << "shell> ";
        std::getline(std::cin, input);

        command.execute_command(input);

    }while(input != "exit");

}
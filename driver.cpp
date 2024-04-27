#include <iostream>
#include "process_manager.h"

int main(){
    std::string input;
    ProcessManager command = ProcessManager();

    do{
        std::cout << "shell> ";
        std::getline(std::cin, input);

        command.execute_command(input);

    }while(input != "exit");

}
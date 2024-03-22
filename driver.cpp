#include <iostream>
#include "run_process.h"

int main(){
    std::string input;
    RunProcess command = RunProcess();

    do{
        std::cout << "shell> ";
        std::getline(std::cin, input);

        command.process(input);

    }while(input != "exit");

}
#include <iostream>
#include "process_control.h"

int main(){
    std::string command;

    do{
        std::cout << "shell> ";
        std::getline(std::cin, command);

        process(command);

    }while(command != "exit");

}
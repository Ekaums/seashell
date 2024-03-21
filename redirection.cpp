#include "redirection.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

void inputRedirect(std::istringstream &inputStream){
    std::string token;
    if(!(inputStream >> token)){ // If no input file given
        std::cerr << "Invalid Command: No input file" << std::endl;
        exit(1);
    }

    int fd = open(token.c_str(), O_RDONLY); // Open file for writing. Clearing any existing text. Specify rwx perms
    if(fd == -1){
        std::cerr << "Error opening input file" << std::endl;
        exit(1);
    }
    if(dup2(fd, STDIN_FILENO) == -1){
        std::cerr << "dup2 error" << std::endl;
        close(fd);
        exit(1);
    }
}

void outputRedirect(std::istringstream &inputStream){
    std::string token;

    if(!(inputStream >> token)){ // If no output file given (reached end of input stream)
        std::cerr << "Invalid Command: No output file" << std::endl;
        exit(1);
    }
    int fd = open(token.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open file for writing, creating it if it does not exist. Clearing any existing text. Specify rwx perms
    if(fd == -1){
        std::cerr << "Error opening output file" << std::endl;
        exit(1);
    }
    if(dup2(fd, STDOUT_FILENO) == -1){
        std::cerr << "dup2 error" << std::endl;
        close(fd);
        exit(1);
    }    
}
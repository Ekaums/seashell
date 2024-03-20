#include "arg_parse.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

// Check if command is valid
static std::string pathAccess(const std::string& token);

std::vector<std::string> parseArgs(const std::string& input){

    std::istringstream inputStream(input); // Define the string as an input stream, to read from
    std::string token;
    inputStream >> token;

    std::string firstToken;

    if((firstToken = pathAccess(token)) == ""){ // Check if command present in /bin or /usr/bin
        std::cerr << "Invalid Command" << std::endl;
        exit(1);
    } 

    std::vector<std::string> exec; // Array of arguments
    exec.push_back(firstToken); // Insert command

    while(inputStream >> token){ // Insert all args

        if(token == ">"){
            if(!(inputStream >> token)){
             // If no output file given (reached end of input stream)
                std::cerr << "Invalid Command: No output file" << std::endl;
                exit(1);
            }
            int fd = open(token.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open file for writing, creating it if it does not exist. Clearing any existing text. Specify rwx perms
            if(fd == -1){
                std::cerr << "Error opening file" << std::endl;
                exit(1);
            }

            if(dup2(fd, STDOUT_FILENO) == -1){
                std::cerr << "dup2 error" << std::endl;
                close(fd);
                exit(1);
            }
            continue;
        }
        else if(token == "<"){
            std::cout << "y" << std::endl;
            continue;
        }
        exec.push_back(token);
    }

    return exec;
}

// Check if valid command
std::string pathAccess(const std::string& token){
    std::string binPathName = "/bin/" + token;
    std::string usrPathName = "/usr/bin/" + token;

    // Check /bin/ and /usr/bin
    if(access(binPathName.c_str(), X_OK) == 0){ 
        return binPathName;
    }
    else if(access(usrPathName.c_str(), X_OK) == 0){
        return usrPathName;
    }

    return "";
}
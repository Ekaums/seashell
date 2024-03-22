#include "run_process.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

void RunProcess::process(const std::string &arg){

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

std::vector<std::string> RunProcess::parseArgs(const std::string& input){

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
            outputRedirect(inputStream);
            continue;
        }   
        else if(token == ">>"){
            outputAppendRedirect(inputStream);
            continue;
        }
        else if(token == "<"){
            inputRedirect(inputStream);
            continue;
        }
        exec.push_back(token);
    }

    return exec;
}

// Check if valid command
std::string RunProcess::pathAccess(const std::string& token){
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

void RunProcess::inputRedirect(std::istringstream &inputStream){
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

void RunProcess::outputRedirect(std::istringstream &inputStream){
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

void RunProcess::outputAppendRedirect(std::istringstream &inputStream){
    std::string token;

    if(!(inputStream >> token)){ // If no output file given (reached end of input stream)
        std::cerr << "Invalid Command: No output file" << std::endl;
        exit(1);
    }
    int fd = open(token.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644); // Open file for writing, creating it if it does not exist. Append to any existing text. Specify rwx perms
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


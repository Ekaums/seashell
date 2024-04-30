#include "process_manager.h"
#include "pipe_manager.h"
#include "job_control.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

void ProcessManager::execute_command(const std::string &arg){

    if(arg == "exit" || arg == ""){
        return;
    }

    if(arg.find('&') != std::string::npos){ // Parallel commands (&)
        JobControl jobManager = JobControl();
        jobManager.process_parallel(arg);
        return;
    }

    if(arg.find('|') != std::string::npos){  // Piping commands (|)
        PipeManager pipeManager = PipeManager();
        pipeManager.execute_pipe_command(arg);
        return;
    }

    int pid = fork(); // All basic commands are simply handled through a fork--exec procedure

    if(pid == 0){ // Child
        setpgid(0, 0); // Place child in its own process group
        std::vector<std::string> args = parse_args(arg);  // Parse input, handling I/O redirection accordingly
        execute_process(args); 
    }
    else{ // Parent
        setpgid(pid, pid); // Ensure child is in it's own process group, if parent executes first. Avoids race condition! 
        waitpid(pid, NULL, 0); // Parent waits until this process completes
    }

    tcsetpgrp(STDIN_FILENO, getpgrp()); // Ensure shell regains control of terminal (may not be needed, but safe to have)
    return;
}


void ProcessManager::execute_process(std::vector<std::string> &args){
        
    char **exec = new char*[args.size()+1];

    for(int i = 0; i < args.size(); i++){
        exec[i] = &args[i][0]; // Insert all args into array
    }

    exec[args.size()+1] = nullptr;

    // call execv with these args
    execv(exec[0], exec);

    std::cerr << "execv error" << std::endl; // execv only returns if an error occured
    std::cerr << strerror(errno) << std::endl;
    exit(1);
}


std::vector<std::string> ProcessManager::parse_args(const std::string& input){
    std::istringstream inputStream(input); // Define the string as an input stream, to read from
    std::string token;
    inputStream >> token;

    std::string firstToken = path_access(token); 

    std::vector<std::string> exec; // Array of arguments
    exec.push_back(firstToken); // Insert command

    while(inputStream >> token){ // Insert all args

        if(token == ">"){
            output_redirect(inputStream);
            continue;
        }   
        else if(token == ">>"){
            output_append_redirect(inputStream);
            continue;
        }
        else if(token == "<"){
            input_redirect(inputStream);
            continue;
        }
        else if(token.at(0) == '\''){
            token.erase(0, 1);

            if(token.back() == '\''){ // Check if the argument is a single word
                token.pop_back();
                exec.push_back(token);
                continue;
            }

            inputStream >> std::noskipws;
            char ch;

            while(inputStream.get(ch)){ // Next argument is all characters in this quote
                if(ch == '\''){
                    break;
                }
                token += ch;
            }

            inputStream >> std::skipws;
        }
        else if(token.at(0) == '\"'){
            token.erase(0, 1);

            if(token.back() == '\"'){ // Check if the argument is a single word
                token.pop_back();
                exec.push_back(token);
                continue;
            }

            inputStream >> std::noskipws;
            char ch;

            while(inputStream.get(ch)){ // Next argument is all characters in this quote
                if(ch == '\"'){
                    break;
                }
                token += ch;
            }

            inputStream >> std::skipws;
        }
        exec.push_back(token);
    }

    return exec;
}

std::string ProcessManager::path_access(const std::string& token){
    std::string binPathName = "/bin/" + token;
    std::string usrPathName = "/usr/bin/" + token;

    // Check if command is in /bin or /usr/bin
    if(access(binPathName.c_str(), X_OK) == 0){ 
        return binPathName;
    }
    else if(access(usrPathName.c_str(), X_OK) == 0){
        return usrPathName;
    }
    else{
        std::cerr << "Invalid Command" << std::endl;
        exit(1);
    }
}

void ProcessManager::input_redirect(std::istringstream &inputStream){
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

void ProcessManager::output_redirect(std::istringstream &inputStream){
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

void ProcessManager::output_append_redirect(std::istringstream &inputStream){
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


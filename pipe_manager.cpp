#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "pipe_manager.h"


void PipeManager::execute_pipe_command(const std::string& pipe_command){

    std::istringstream input(pipe_command);
    std::string command;
    ProcessManager processManager = ProcessManager();

    int dup_stdin = dup(STDIN_FILENO); // Save terminal output fd, to map output to after we are done piping

    while(std::getline(input, command, '|')){ // For every pipe command
        if (!input.eof()){ // (Until the last one)
            int fd[2];
            pipe(fd);
            int rc = fork();

            if(rc == 0){ // Child
                std::vector<std::string> args = processManager.parse_args(command); // Parse args
                if(args.empty()){
                    exit(1);
                }
                dup2(fd[1], STDOUT_FILENO); // Redirect output to write to pipe, which will be used as input to next commmand
                processManager.execute_process(args);
            }
            else{
                dup2(fd[0], STDIN_FILENO); // Parent will set up input fd to read from this pipe, such that the next process can use this data
                // Cleanup
                close(fd[0]);
                close(fd[1]);
            }
        }
    }

    // The last command is handled seperately, as it's output is redirected to the terminal instead of another pipe
    int rc = fork();

    // More of the same business
    if(rc == 0){
        std::getline(input, command);
        std::vector<std::string> args = processManager.parse_args(command);
        if(args.empty()){
            exit(1);
        }
        processManager.execute_process(args);
    }
    else{
        int status;
        while(wait(&status) > 0); // Parent will prompt user again once ALL processes in the pipe are done
        dup2(dup_stdin, STDIN_FILENO);
        close(dup_stdin);
    }

    return;
}
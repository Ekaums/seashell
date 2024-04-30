#include "job_control.h"
#include "process_manager.h"
#include <signal.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

void JobControl::process_parallel(const std::string &arg){

    std::istringstream input(arg);
    std::string command;
    ProcessManager processManager = ProcessManager();

     while(std::getline(input, command, '&')){
        if (input.eof()){ // Process all commands until the last one
            break;
        }

        if(fork() == 0){
            setpgid(0, 0); // Every process is placed into its own process group, for job control
            std::vector<std::string> args = processManager.parse_args(command); // Parse args
            processManager.execute_process(args);
        }
    }

    if(command == ""){ // If the last token in the command is '&', there is nothing left to parse
        return;
    }

    int rc = fork(); // Otherwise, the last command is a foreground job. User must wait for this process to complete before being doing anything

    if(rc == 0){
        setpgid(0, 0);
        std::vector<std::string> args = processManager.parse_args(command);
        processManager.execute_process(args);
    }
    else{
        waitpid(rc, NULL, 0); // Wait for this foreground job to complete
    }

    tcsetpgrp(STDIN_FILENO, getpgrp());
    return;
}

void JobControl::init_parallel(void){
    struct sigaction sa;
    sa.sa_handler = handle_SIGCHLD;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);
}

void JobControl::handle_SIGINT(int sig){

}
    
void JobControl::handle_SIGCHLD(int sig){
    int status;
    int pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
             // std::cout << "Child " << pid <<  " terminated with status: " << WEXITSTATUS(status) << std::endl;
        } 
        else if (WIFSIGNALED(status)) {
            // std::cout << "Child " << pid << " killed by signal: " << WTERMSIG(status) << std::endl;
        } 
        else if (WIFSTOPPED(status)) {
            // std::cout << "child stopped" << std::endl;
        } 
        else if (WIFCONTINUED(status)) {
            // std::cout << "child continued" << std::endl;
        }
    }
}
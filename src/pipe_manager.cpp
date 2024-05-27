#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "pipe_manager.h"
#include "process_manager.h"
#include "job_control.h"


void PipeManager::execute_pipe_command(const std::string& pipe_command){

    std::istringstream input(pipe_command);
    std::string command;
    ProcessManager processManager = ProcessManager();

    int dup_stdin = dup(STDIN_FILENO); // Save terminal output fd, to map output to after we are done piping

    // Piping commands are all placed in the same process group, to handle as one "command"
    bool first = false;
    int group_pid;

    jobby.block_SIGCHLD();

    while(std::getline(input, command, '|')){ // For every pipe command...
        if (input.eof()){ // until the last one
            break;
        }
            int fd[2];
            pipe(fd);
            int rc = fork();

            if(first == false){ // Process group ID is set as the process ID of the first command (arbitrarily)
                if(rc == 0){ 
                    group_pid = getpid(); // Child saves it's own process ID
                }
                else{
                    group_pid = rc; // Parent saves child's ID
                }
                first = true;
            }

            if(rc == 0){
                setpgid(0, group_pid); // Set child's PGID to this group ID
                std::vector<std::string> args = processManager.parse_args(command);
                dup2(fd[1], STDOUT_FILENO); // Redirect output to write to pipe, which will be used as input to next commmand
                processManager.execute_process(args);
            }
            else{
                setpgid(rc, group_pid); // Set child's PGID to this group ID

                jobby.init_job(command, rc, group_pid, jobby.get_next_jobID(), JobState::running, false);

                dup2(fd[0], STDIN_FILENO); // Parent will set up input fd to read from this pipe, such that the next process can use this data
                // Cleanup
                close(fd[0]);
                close(fd[1]);
            }
    }

    // The last command is handled seperately, as it's output is redirected to the terminal instead of another pipe
    std::getline(input, command);
    int rc = fork();

    // More of the same business
    if(rc == 0){
        setpgid(0, group_pid);
        std::vector<std::string> args = processManager.parse_args(command);
        processManager.execute_process(args);
    }
    else{
        setpgid(rc, group_pid);

        jobby.init_job(command, rc, group_pid, jobby.get_next_jobID(), JobState::running, false);

        tcsetpgrp(STDIN_FILENO, group_pid); // Set child pgroup as foreground group

        jobby.unblock_SIGCHLD();

        waitpid(-group_pid, NULL, 0); // Parent waits until all processes in process group (all commands in the pipe) complete
        
        signal(SIGTTOU, SIG_IGN); 
        tcsetpgrp(STDIN_FILENO, getpgrp());
        signal(SIGTTOU, SIG_DFL); 

        // After done  piping, reset output to terminal
        dup2(dup_stdin, STDIN_FILENO);
        close(dup_stdin);
    }

    return;
}

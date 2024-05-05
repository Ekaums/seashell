#include "job_control.h"
#include "process_manager.h"
#include <signal.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

JobControl jobby = JobControl();

void JobControl::init_job(std::string command, pid_t pgid, int jobID, JobState state, bool isBG){
    Job newJob = {
        command,
        pgid,
        jobID,
        state,
        isBG,
    };

    JobList.push_back(newJob);
}

void JobControl::init_parallel(void){
    struct sigaction sa;
    sa.sa_handler = handle_SIGCHLD;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);
}


void JobControl::listJobs(){
    for(const auto& job : JobList){
        std::cout << "Job [" << job.jobID << "]: " << job.command << " [" << ((bool)job.state ? "Running" : "Stopped") << "]" << std::endl;
    }
}



int JobControl::get_next_jobID(void){
    int id;

    if(availableIDs.empty()){
        id = nextJobID++;
    }
    else{
        id = availableIDs.front();
        availableIDs.pop();
    }

    activeIDs.insert(id);
    return id;
}

void JobControl::recycle_jobID(int id){
    activeIDs.erase(id);
    availableIDs.push(id);
}

    
void JobControl::handle_SIGCHLD(int sig){
    int status;
    int pid;
    bool enter = false;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        enter = true;
        if (WIFEXITED(status)) {
            // Terminated
        } 
        else if (WIFSIGNALED(status)) {
            if(WTERMSIG(status) == SIGINT){
                std::cout << std::endl; // Child killed by Ctrl-C
            }
            else{
                std::cout << "Child killed somehow" << std::endl;
            }
        } 
        else if (WIFSTOPPED(status)) {
            if(WSTOPSIG(status) == SIGTSTP){
                std::cout << std::endl; // Child suspended by Ctrl-Z
            }
            else{
                std::cout << "child paused somehow" << std::endl;
            }
        } 
        else if (WIFCONTINUED(status)) {
            std::cout << "child continued" << std::endl;
        }
    }
    if(!enter){
        std::cout << std::endl;
    }
}


void JobControl::block_SIGCHLD(void){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigset, NULL);    
}


void JobControl::unblock_SIGCHLD(void){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);  
}

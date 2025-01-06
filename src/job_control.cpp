#include <signal.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "../include/job_control.h"
#include "../include/process_manager.h"

JobControl jobby;

void JobControl::init_job(std::string command, pid_t pid, pid_t pgid, int jobID, JobState state, bool isBG){
    Job newJob = {
        command,
        pid,
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

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status) || // Child completed
        (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)){ // Child killed by Ctrl-C
            for(auto it = jobby.JobList.begin(); it != jobby.JobList.end(); ){
                if(it->pid == pid){
                    jobby.recycle_jobID(it->jobID);
                    jobby.JobList.erase(it);
                }
                else{
                    it++;
                }
            }
        } 
        else if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTSTP){ // Child suspended by Ctrl-Z
            std::cout << std::endl;
            for(auto it = jobby.JobList.begin(); it != jobby.JobList.end(); ){
                if(it->pid == pid){
                    it->state = JobState::stopped;
                }
            }
        } 
        else if (WIFCONTINUED(status)){
            std::cout << "child continued" << std::endl;
        }
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

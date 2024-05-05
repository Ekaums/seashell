#pragma once
#include <string>
#include <set>
#include <queue>

enum class JobState{
    stopped,
    running,
    terminated,
};

struct Job{
    std::string command;
    pid_t pgid;
    int jobID;
    JobState state;
    bool isBG;
};

class JobControl{

    private:
        std::set<int> activeIDs;
        std::queue<int> availableIDs;
        int nextJobID = 1;

        // Signal handlers must be static as signals do not operate within the context of a class (do not use instance-specfic data)
        // Also because classes are not a C concept 
        static void handle_SIGINT(int sig);
        static void handle_SIGCHLD(int sig);

        void recycle_jobID(int id);

    public:
        std::vector<Job> JobList;

        // Setup signal handlers
        void init_parallel(void);

        int get_next_jobID();

        void block_SIGCHLD();
        void unblock_SIGCHLD();

        void init_job(std::string command, pid_t pgid, int jobID, JobState state, bool isBG);

        void listJobs();

};

extern JobControl jobby;
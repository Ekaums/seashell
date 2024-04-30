#pragma once
#include <string>

class JobControl{

     // Friends with ProcessManager, in order to parse arguments and run processes

    private:
        // Signal handlers must be static as signals do not operate within the context of a class (do not use instance-specfic data)
        // Also because classes are not a C concept 
        static void handle_SIGINT(int sig);
        static void handle_SIGCHLD(int sig);

    public:
        // Setup signal handlers
        void init_parallel(void);

        // Handle background commands
        void process_parallel(const std::string &arg);

};
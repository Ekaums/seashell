#pragma once
#include <string>

class PipeManager{
    
    // Friends with ProcessManager, in order to parse arguments and run processes

    public:
        void execute_pipe_command(const std::string& pipe_command);
};

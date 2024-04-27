#pragma once

#include <string>
#include "process_manager.h"

class PipeManager{
    
    // Friends with ProcessManager, in order to parse arguments and run processes

    public:
        void execute_pipe_command(const std::string& pipe_command);
};
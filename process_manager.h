#pragma once
#include <string>


class ProcessManager{

    private:
        // Check if command is valid
        std::string path_access(const std::string& token);

        // IO redirection
        void input_redirect(std::istringstream &inputStream);
        void output_redirect(std::istringstream &inputStream);
        void output_append_redirect(std::istringstream &inputStream);

        // Parse arguments, setting up env to execute command
        std::vector<std::string> parse_args(const std::string& input);

        // All simple foreground commands handled here
        void process_foreground_command(const std::string &arg);

        // Run the process
        void execute_process(std::vector<std::string> &args);

    public:
        // Handle user input
        void process_command(const std::string &arg);
    
    friend class PipeManager; // Pipe commands will similarily be parsed and executed
    friend class JobControl; // Parallel execution as well
};
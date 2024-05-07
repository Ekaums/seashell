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

        // Simple foreground commands handled here
        void process_foreground_command(const std::string &command);

        // Run the process
        void execute_process(std::vector<std::string> &commands);

    public:
        // Handle user input
        void process_command(const std::string &command);

        // Handle background commands
        void process_parallel(const std::string &command);
    
    friend class PipeManager; // Pipe commands will similarily be parsed and executed
};

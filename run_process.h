#include <string>


class RunProcess{
    private:
        // Check if command is valid
        std::string path_access(const std::string& token);

        // Parse arguments, setting up env to execute command
        std::vector<std::string> parse_args(const std::string& input);

        // IO redirection
        void input_redirect(std::istringstream &inputStream);
        void output_redirect(std::istringstream &inputStream);
        void output_append_redirect(std::istringstream &inputStream);

    public:
        // Create new process for command
        void execute_process(const std::string &arg);
};
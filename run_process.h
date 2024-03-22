#include <string>


class RunProcess{
    private:
        // Check if command is valid
        std::string pathAccess(const std::string& token);

        // Parse arguments, setting up env to execute command
        std::vector<std::string> parseArgs(const std::string& input);

        // IO redirection
        void inputRedirect(std::istringstream &inputStream);
        void outputRedirect(std::istringstream &inputStream);
        void outputAppendRedirect(std::istringstream &inputStream);

    public:
        // Create new process for command
        void process(const std::string &arg);
};
#include "main.hpp"


int main(int numberOfArguments, char **commandArguments)
{
    //Create a StrataConfig object pointer to the singleton to discover what game
    //we are extracting and what "rules" to follow.
    StrataConfig *Config = StrataConfig::Configuration();
    
    std::cout << "StrataExtract CommandLine\n";
    std::cout << "Hardware Cores: " << StrataConfig::GetCPUCores() << '\n';
    Config->readConfig("/Users/brad/Desktop/SampleConfig.StrataExtract");
    
    
    std::cout << "Progress: " << Config->GetProgess();
    return 0;
}
#include "StrataRunner.hpp"

StrataRunner::StrataRunner()
{
    config = StrataConfig::Configuration();
}

StrataRunner* StrataRunner::runnerInstance = 0;

StrataRunner *StrataRunner::Runner()
{
    if(runnerInstance == 0)
    {
        runnerInstance = new StrataRunner;
    }
    
    return runnerInstance;
}

void StrataRunner::run()
{
    if(!config->isConfigLoaded())
    {
        std::cout << "!!!Config NOT Loaded!!!\n";
    }
}
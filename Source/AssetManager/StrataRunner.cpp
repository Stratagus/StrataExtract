#include "StrataRunner.hpp"

StrataRunner::StrataRunner()
{
    loadedConfiguration = NULL;
}

StrataRunner::~StrataRunner()
{
    
}
bool StrataRunner::SetConfiguration(StrataConfig *targetConfiguration)
{
    if(!targetConfiguration)
    {
        #warning Throw no config initialized here
        return false;
    }
    if(!targetConfiguration->isConfigLoaded())
    {
        #warning Throw no config loaded
        return false;
    }
    loadedConfiguration = targetConfiguration;
    return true;
}

void StrataRunner::SpawnThreads()
{
    if(!loadedConfiguration)
    {
    #warning Throw no config initialized here
        return false;
    }
}
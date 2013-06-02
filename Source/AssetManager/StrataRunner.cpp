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

bool StrataRunner::SpawnThreads()
{
    if(!loadedConfiguration)
    {
    	#warning Throw no config initialized here
    	BOOST_LOG_SEV(runnerLogger, boost::log::trivial::error) << "No StrataExtract configuration loaded!";
    	return false;
    }
    for(int currentNewThread; currentNewThread < loadedConfiguration->GetCPUCores();currentNewThread++)
    {
    	BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << "Spawning Thread #" << currentNewThread;
    	threadPool.push_back(new boost::thread());
    }
    return true;
}

#include "StrataRunner.hpp"

StrataRunner::StrataRunner()
{
    loadedConfiguration = nullptr;
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
    for(int currentNewThread = 0; currentNewThread < loadedConfiguration->GetCPUCores();currentNewThread++)
    {
    	BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << "Spawning Thread #" << currentNewThread;
    	//boost::thread *currentThread = new boost::thread(boost::bind(&StrataRunner::Temp, this));
//boost::thread t(boost::bind(&SocketServer::runServer, this));
    	threadPool.push_back(new boost::thread());
    	//threadPool[0]->run(boost::bind(&StrataRunner::Temp, this));
    }

    return true;
}

bool StrataRunner::JoinThreads()
{
	for(int currentJoinThread = 0; currentJoinThread < threadPool.size(); currentJoinThread++)
	{
		threadPool[currentJoinThread]->join();
		BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << "Joining Thread #" << currentJoinThread;

	}
	threadPool.erase(threadPool.begin(), threadPool.end());
}

bool StrataRunner::ProcessPrerequisites()
{
	if(!loadedConfiguration)
	{
		#warning Throw no config initialized here
		BOOST_LOG_SEV(runnerLogger, boost::log::trivial::error) << "No StrataExtract configuration loaded!";
	return false;
	}
	if(!boost::filesystem::exists(loadedConfiguration->GameMediaDestinationPath()))
	{
		BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << "GameMediaDestinationPath does not exist - Creating";
		boost::filesystem::create_directory(loadedConfiguration->GameMediaDestinationPath());

	}
	BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << loadedConfiguration->preparationProcessQueue.size()
															<< " Prerequisite Job(s) to complete";
	while(loadedConfiguration->preparationProcessQueue.size())
	{
		ProcessAsset(loadedConfiguration->preparationProcessQueue.front());
		loadedConfiguration->preparationProcessQueue.pop();
        BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << "Compeleted Prerequisite Job";
	}
    
    return true;
}

bool StrataRunner::ProcessAssets(xmlNodePtr assetList)
{
	xmlNodePtr currentProcess = assetList;
	while(currentProcess != nullptr)
	{
        BOOST_LOG_SEV(runnerLogger, boost::log::trivial::trace) << "Pre-Processing: " << (char *) currentProcess->name;
		currentProcess = currentProcess->next;
	}
	return true;
}

#warning Feels like a hacky solution, when in truth it's the same as ProcessAssets but only the first object
bool StrataRunner::ProcessAsset(xmlNodePtr assetObject)
{
    //if(xmlStrcmp(xmlGetProp(assetObject, (xmlchar *) "name"), ())
    return true;
}

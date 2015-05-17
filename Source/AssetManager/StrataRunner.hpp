#pragma once

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>
#include <boost/exception/all.hpp>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "StrataRunnerException.hpp"

#include "../Configuration/StrataConfig.hpp"
#include "../Archives/StrataArchive.hpp"
#include "../Conversion/Audio/StrataAudio.hpp"
#include "../Conversion/Image/StrataImage.hpp"
#include "../Conversion/Video/StrataVideo.hpp"

class StrataRunner
{
    public:
        StrataRunner();
        ~StrataRunner();
        bool SetConfiguration(StrataConfig *targetConfiguration);
        bool SpawnThreads();
        bool JoinThreads();

        bool ProcessPrerequisites();
        bool ProcessAssets(xmlNodePtr assetList);
        bool ProcessAsset(xmlNodePtr assetObject);

    protected:

        std::vector<boost::thread *> threadPool;
        //The Configuration logger to help for tracing
        boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> runnerLogger;
    private:
        StrataConfig *loadedConfiguration;
};

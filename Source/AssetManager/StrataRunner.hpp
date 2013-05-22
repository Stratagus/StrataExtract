#ifndef StrataRunner_Header
#define StrataRunner_Header

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
    protected:
    private:
        StrataConfig *config;
};

#endif
/*! \mainpage StrataExtract Documentation
 *  \section Into Introduction
 *  Welcome to StrataExtract, the all-in-one tool for game asset extraction
 *  and Stratagus configuration. This will allow for less fragmentation and
 *  better overall media support.
 *  \setion PlatformSupport Platform Support
 *  Current target platforms include
 *  Windows Vista+
 *  Mac OSX 10.5+
 *  *nix
 */

#ifndef Main_Header
#define Main_Header
#include "SimpleOpt.h"

#include "Configuration/StrataConfig.hpp"


// The valid terminal options
enum
{
    CMDOPTION_Help,
    CMDOPTION_GameConfigurationFilePath,
    CMDOPTION_GameMediaSourceFilePath,
    CMDOPTION_GameMediaDestinationFilePath,
    CMDOPTION_SimulateExtraction,
    CMDOPTION_Verbosity,
    
};

const CSimpleOpt::SOption CommandLineOptions[] =
{
    {CMDOPTION_Help,                            "-h",               SO_NONE},
    {CMDOPTION_Help,                            "--help",           SO_NONE},
    
    {CMDOPTION_GameConfigurationFilePath,       "-c",               SO_REQ_SEP},
    {CMDOPTION_GameConfigurationFilePath,       "--config",         SO_REQ_SEP},
    
    {CMDOPTION_GameMediaSourceFilePath,         "-g",               SO_REQ_SEP},
    {CMDOPTION_GameMediaSourceFilePath,         "--game",           SO_REQ_SEP},
    
    {CMDOPTION_GameMediaDestinationFilePath,    "-d",               SO_REQ_SEP},
    {CMDOPTION_GameMediaDestinationFilePath,    "--dest",           SO_REQ_SEP},
    
    {CMDOPTION_SimulateExtraction,              "-s",               SO_NONE},
    {CMDOPTION_SimulateExtraction,               "--simulate",      SO_NONE},
    
    {CMDOPTION_Verbosity,                       "-v",               SO_REQ_SEP},
    {CMDOPTION_Verbosity,                       "--verbosity",      SO_REQ_SEP},
};

#endif
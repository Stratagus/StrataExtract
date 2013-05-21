#ifndef StrataConfig_UnitTests_Header
#define StrataConfig_UnitTests_Header

#include <boost/test/unit_test.hpp>
#include "../../Source/Configuration/StrataConfig.hpp"


//Set the value to a  valid game media directory
//If Apple OSX
#ifdef __APPLE__
    #define GAMESDIRECTORY "/Users/brad/Games"
    #define GAMEDESTINATIONDIRECTORY "/Users/brad/Desktop/"
    #define GAMECONFIGFILEPATH "../../Docs/SampleConfigs/Stargus.StrataExtract"
    //Set to the UnitTestMedia Junkfile path
    #define BADGAMECONFIGFILEPATH "../../Docs/UnitTestMedia/junkfile"

#else
//If the OS is something else
#define GAMESDIRECTORY "/home/brad/Games"
#define GAMEDESTINATIONDIRECTORY "/home/brad/Desktop/"
#define GAMECONFIGFILEPATH "../Docs/SampleConfigs/Stargus.StrataExtract"
//Set to the UnitTestMedia Junkfile path
#define BADGAMECONFIGFILEPATH "../Docs/UnitTestMedia/junkfile"

#endif



struct StrataConfigInstance
{
    StrataConfig *myConfiguration;
    
    
    StrataConfigInstance()
    {
        BOOST_TEST_MESSAGE("Setup StrataConfiguration Tests");
            myConfiguration = new StrataConfig;
    }
    
    ~StrataConfigInstance()
    {
        BOOST_TEST_MESSAGE("Teardown StrataConfiguration Tests");
        if(myConfiguration)
        {
            delete myConfiguration;
            myConfiguration = NULL;
        }
    }
};


#endif
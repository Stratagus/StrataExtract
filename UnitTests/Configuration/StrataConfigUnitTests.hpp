#ifndef StrataConfig_UnitTests_Header
#define StrataConfig_UnitTests_Header

#include <boost/test/unit_test.hpp>
#include "../../Source/Configuration/StrataConfig.hpp"


//Set the value to a  valid game media directory
#define GAMESDIRECTORY "/Users/brad/Games"
#define GAMEDESTINATIONDIRECTORY "/Users/brad/Desktop/"
#define GAMECONFIGFILEPATH "../../Docs/SampleConfigs/Stargus.StrataExtract"
#define BADGAMECONFIGFILEPATH "../../Docs/UnitTestMedia/junkfile"

struct StrataConfigInstance
{
    StrataConfig *myConfiguration = NULL;
    
    
    StrataConfigInstance()
    {
        BOOST_TEST_MESSAGE("Setup StrataConfiguration Tests");
        if(!myConfiguration)
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
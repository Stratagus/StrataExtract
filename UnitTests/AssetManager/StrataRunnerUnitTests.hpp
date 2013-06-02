#ifndef StrataRunner_UnitTests_Header
#define StrataRunner_UnitTests_Header

#include <boost/test/unit_test.hpp>
#include "../../Source/AssetManager/StrataRunner.hpp"
#include "../Configuration/StrataConfigUnitTests.hpp"

struct StrataRunnerInstance
{
    StrataConfig *myConfiguration;


    StrataRunnerInstance()
    {
        BOOST_TEST_MESSAGE("Setup StrataRunner Configuration Object");
            myConfiguration = new StrataConfig;
            myConfiguration->setGameConfiguration(GAMECONFIGFILEPATH);
            myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
            myConfiguration->setGameMediaDestinationPath(GAMEDESTINATIONDIRECTORY);
            myConfiguration->readConfig();

    }

    ~StrataRunnerInstance()
    {
        BOOST_TEST_MESSAGE("Teardown StrataRunner Configuration Object");
        if(myConfiguration)
        {
            delete myConfiguration;
            myConfiguration = NULL;
        }
    }
};
#endif

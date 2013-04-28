#include "StrataConfigUnitTests.hpp"

BOOST_FIXTURE_TEST_SUITE(StrataConfigTests, StrataConfigInstance)

BOOST_AUTO_TEST_CASE( ReadError )
{
    //The root directory of the soure game
    if(myConfiguration->setGameMediaSourcePath("/Users/brad/Games/"))
    {
        std::cout << "Set Source path\n";
    }
    else
    {
        std::cout << "SOURCE PATH not set\n";
    }
    
    //Where we want the resulting file to be
    if(myConfiguration->setGameMediaDestinationPath("/Users/brad/Desktop/Game"))
    {
        std::cout << "Set Destination path\n";
    }
    else
    {
        std::cout << "DESTINATION PATH not set\n";
    }
    
    
    if(myConfiguration->setGameConfiguration("../../Docs/SampleConfigs/Stargus.StrataExtract"))
    {
        std::cout << "Set Configuration path\n";
    }
    else
    {
        std::cout << "CONFIG PATH not set\n";
    }
    //Read the XML configuration
    try
    {
            myConfiguration->readConfig();
    }
    catch(...)
    {
        
    }
}

BOOST_AUTO_TEST_SUITE_END()
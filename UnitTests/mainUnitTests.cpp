#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include "mainUnitTests.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MiscTests)

/*BOOST_AUTO_TEST_CASE( Play )
{
    std::cout << "StrataConfig Tests\n";
    //Create a Strataconfig reference (from teh singleton
    StrataConfig *myConfig = new StrataConfig;
    
    //The root directory of the soure game
    if(myConfig->setGameMediaSourcePath("/Users/brad/Games/"))
    {
        std::cout << "Set Source path\n";
    }
    else
    {
        std::cout << "SOURCE PATH not set\n";
    }
    
    //Where we want the resulting file to be
    if(myConfig->setGameMediaDestinationPath("/Users/brad/Desktop/"))
    {
        std::cout << "Set Destination path\n";
    }
    else
    {
        std::cout << "DESTINATION PATH not set\n";
    }
    
    
    myConfig->setGameConfiguration("../../Docs/SampleConfigs/Stargus.StrataExtract");
    //Read the XML configuration
    try
    {
        myConfig->readConfig();
    }
    catch(...)
    {
        
    }
    
    if(myConfig->isExpansionGame())
    {
        std::cout << "You are loading a expansion game, you must give a proper destination path.\n";
    }
    else
    {
        std::cout << "Not a Expansion.\n";
    }
    myConfig->ProcessGameAssetLists();
    //std::cout << "Game Name: " << myConfig->getGameName() << '\n';
    //StrataRunner *runner = StrataRunner::Runner();
    std::cout << "Done!\n";
}*/

BOOST_AUTO_TEST_SUITE_END()

#include "mainUnitTests.hpp"

MiscTests::MiscTests() : CppUnit::TestCase("Miscellaneous Tests")
{
}

int main()
{
	std::cout << "StrataExtract Core Unit Tests\n";
    
    CppUnit::TextUi::TestRunner runner;
    
    //Main Object
    
    //runner.addTest( SampleUnitTest::suite() );
    runner.addTest(MiscTests::suite());
    
    runner.run();
	return 0;
}

void MiscTests::test1()
{
    std::cout << "StrataConfig Tests\n";
    //Create a Strataconfig reference (from teh singleton
    StrataConfig *myConfig = StrataConfig::Configuration();
    
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
    
    //Read the XML configuration
    myConfig->readConfig("../../Docs/SampleConfigs/Stargus.StrataExtract");
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
    StrataRunner *runner = StrataRunner::Runner();
    std::cout << "Done!\n";
}
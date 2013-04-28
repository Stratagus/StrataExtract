#include "StrataConfigUnitTests.hpp"

BOOST_FIXTURE_TEST_SUITE(StrataConfigTests, StrataConfigInstance)

BOOST_AUTO_TEST_CASE(GameMediaSourcePathBAD)
{
    try
    {
        myConfiguration->setGameMediaSourcePath("/some/bad/path");
        BOOST_CHECK(false);
    }
    catch (StrataConfigFilesystemException &errorInstance)
    {
        //Cout or log the error message with GetErrorMessage()
        //std::cout << "File Error Message: " << errorInstance.GetErrorMessage();
        
        //It is possible to force override the checks here with (thought it's a really bad idea)
        //Instead call the setter again with the new value.
        //*errorInstance.problemPath = "/some/new/path";
        
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
        BOOST_CHECK(true);

    }
    catch (...)
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(GameMediaSourcePathGOOD)
{
    try
    {
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
    }
    catch(...)
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(GameMediaDestinationPathBAD)
{
    try
    {
        myConfiguration->setGameMediaSourcePath("/some/bad/path");
        BOOST_CHECK(false);
    }
    catch (StrataConfigFilesystemException &errorInstance)
    {
        //Cout or log the error message with GetErrorMessage()
        //std::cout << "File Error Message: " << errorInstance.GetErrorMessage();
        
        //It is possible to force override the checks here with (thought it's a really bad idea)
        //Instead call the setter again with the new value.
        //*errorInstance.problemPath = "/some/new/path";
        
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
        BOOST_CHECK(true);
        
    }
    catch (...)
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(GameMediaDestinationPathGOOD)
{
    try
    {
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
    }
    catch(...)
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE( ParserReadError )
{
    try
    {
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
        myConfiguration->setGameMediaDestinationPath(GAMEDESTINATIONDIRECTORY);
        myConfiguration->setGameConfiguration(GAMECONFIGFILEPATH);
        myConfiguration->readConfig();
        BOOST_CHECK(true);
    }
    catch(StrataConfigParsingException &errorInstance)
    {
        //Exceptions specific to Configuration Parsing
        std::cout << "There was an error: "  << errorInstance.GetErrorMessage();
        BOOST_CHECK(true);
    }
    catch(StrataConfigException &errorInstance)
    {
        //Exceptions base on anything strataconfig
        std::cout << "There was an error: "  << errorInstance.GetErrorMessage();
        BOOST_CHECK(false);
    }
    catch(...)
    {
        //Catch Anything
        BOOST_REQUIRE(false);
    }
}

BOOST_AUTO_TEST_SUITE_END()
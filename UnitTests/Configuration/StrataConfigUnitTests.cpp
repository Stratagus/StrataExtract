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
        //std::cout << errorInstance.GetErrorMessage();
        
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

BOOST_AUTO_TEST_CASE(GameDestinationPathBAD)
{
    try
    {
        myConfiguration->setGameMediaSourcePath("/some/bad/path");
        BOOST_CHECK(false);
    }
    catch (StrataConfigFilesystemException &errorInstance)
    {
        //Cout or log the error message with GetErrorMessage()
        //std::cout << errorInstance.GetErrorMessage();
        
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

BOOST_AUTO_TEST_CASE(GameMediaDestinationGOOD)
{
    try
    {
        myConfiguration->setGameMediaDestinationPath(GAMEDESTINATIONDIRECTORY);
    }
    catch(...)
    {
        BOOST_CHECK(false);
    }
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE( ParserReadError )
{
    //The root directory of the source game
    
    try
    {
        myConfiguration->setGameConfiguration(BADGAMECONFIGFILEPATH);
        BOOST_CHECK_THROW(myConfiguration->readConfig(), StrataConfigParsingException);
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

BOOST_AUTO_TEST_CASE(FindGameTitleGood)
{
    try
    {
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
        myConfiguration->setGameConfiguration(GAMECONFIGFILEPATH);
        myConfiguration->readConfig();
        std::cout << "The Game Version is " << myConfiguration->getGameName() << '\n';
        BOOST_CHECK(myConfiguration->isConfigLoaded());
        //BOOST_CHECK_THROW(myConfiguration->readConfig(), StrataConfigParsingException);
    }
    catch(StrataConfigException &errorInstance)
    {
        //Exceptions base on anything strataconfig
        std::cout  << '\n' << errorInstance.GetErrorMessage() << '\n';
        BOOST_CHECK(false);
    }
    catch(...)
    {
        //Catch Anything
        BOOST_REQUIRE(false);
    }
}

BOOST_AUTO_TEST_CASE(CheckForGoodExpansion)
{
    try
    {
        myConfiguration->setGameMediaSourcePath(GAMESEXPANSIONDIRECTORY);
        myConfiguration->setGameConfiguration(GAMECONFIGFILEPATH);
        myConfiguration->readConfig();
        BOOST_CHECK(myConfiguration->isExpansionGame());

        //BOOST_CHECK_THROW(myConfiguration->readConfig(), StrataConfigParsingException);
    }
    catch(StrataConfigException &errorInstance)
    {
        //Exceptions base on anything strataconfig
        std::cout  << '\n' << errorInstance.GetErrorMessage() << '\n';
        BOOST_CHECK(false);
    }
    catch(...)
    {
        //Catch Anything
        BOOST_REQUIRE(false);
    }
}

BOOST_AUTO_TEST_CASE(ProcessGoodArchive)
{
    try
    {
        myConfiguration->setGameMediaSourcePath(GAMESDIRECTORY);
        myConfiguration->setGameConfiguration(GAMECONFIGFILEPATH);
        myConfiguration->setGameMediaDestinationPath(GAMEDESTINATIONDIRECTORY);
        myConfiguration->readConfig();
        myConfiguration->ProcessGameAssetLists();
    }
    catch(StrataConfigException &errorInstance)
    {
        //Exceptions base on anything strataconfig
        std::cout  << '\n' << errorInstance.GetErrorMessage() << '\n';
        BOOST_CHECK(false);
    }
    catch(...)
    {
        //Catch Anything
        BOOST_REQUIRE(false);
    }
}
BOOST_AUTO_TEST_SUITE_END()
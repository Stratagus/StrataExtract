#include "StrataConfigUnitTests.hpp"

BOOST_FIXTURE_TEST_SUITE(StrataConfigTests, StrataConfigInstance)

BOOST_AUTO_TEST_CASE( ReadError )
{
    
    myConfiguration->setGameConfiguration("../../../Docs/SampleConfigs/Stargus.StrataExtract");
}

BOOST_AUTO_TEST_SUITE_END()
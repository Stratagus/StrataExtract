#ifndef Main_UnitTests_Header
#define Main_UnitTests_Header

#include <boost/test/unit_test.hpp>
#include "../../Source/Configuration/StrataConfig.hpp"

BOOST_AUTO_TEST_SUITE(StrataConfigUnitTests)

BOOST_AUTO_TEST_CASE( ReadError )
{
    
    BOOST_CHECK( 2 == 1 );
}

BOOST_AUTO_TEST_SUITE_END()
#endif
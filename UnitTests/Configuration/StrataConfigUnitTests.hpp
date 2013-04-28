#ifndef Main_UnitTests_Header
#define Main_UnitTests_Header

#include <boost/test/unit_test.hpp>
#include "../../Source/Configuration/StrataConfig.hpp"

struct StrataConfigInstance
{
    StrataConfig *myConfiguration;
    
    StrataConfigInstance()
    {
        BOOST_TEST_MESSAGE("setup mass");
        myConfiguration = StrataConfig::Configuration();
    }
    
    ~StrataConfigInstance()
    {
        BOOST_TEST_MESSAGE("teardown mass");
    }
};


#endif
#ifndef Main_UnitTests_Header
#define Main_UnitTests_Header

#include <boost/test/unit_test.hpp>
#include "../../Source/Configuration/StrataConfig.hpp"

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
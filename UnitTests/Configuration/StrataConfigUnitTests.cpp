#include "StrataConfigUnitTests.hpp"

StrataConfigUnitTests::StrataConfigUnitTests() : CppUnit::TestCase("StrataExtract Configuration Tests")
{
     myConfig = StrataConfig::Configuration();
}

void StrataConfigUnitTests::noConfigurationFile()
{
    myConfig->setGameConfiguration("/Users/brad/dev/strata/strataextract/build/CMakeCache.txt");
    try {
        myConfig->readConfig();
        
    }
    catch(StrataConfigParsingException &error)
    {
        CPPUNIT_ASSERT(true);
    }
    catch (...)
    {
        CPPUNIT_ASSERT(false);
    }
    
}

// Actual test case functions and code
void StrataConfigUnitTests::testObviousOne()
{
	CPPUNIT_ASSERT( 1 == 1);
}


#include "StrataConfigUnitTests.hpp"

SampleUnitTest::SampleUnitTest() : CppUnit::TestCase("Sample UnitTest")
{
}

// Actual test case functions and code
void SampleUnitTest::testObviousOne()
{
	CPPUNIT_ASSERT( 1 == 1);
}

void SampleUnitTest::testObviousTwo()
{
    CPPUNIT_ASSERT( 2 == 2);
}

void SampleUnitTest::testObviousThree()
{
    CPPUNIT_ASSERT( 3 == 3);
}

void SampleUnitTest::testObviousFour()
{
    CPPUNIT_ASSERT( 3 == 4);  // Oh no! This test will fail.
}
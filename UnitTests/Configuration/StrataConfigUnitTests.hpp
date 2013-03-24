/*!
 *  \brief
 *  \details
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      March 24, 2013
 *  \copyright GPLv2
 */

#ifndef StrataConfig_UnitTests_Header
#define StrataConfigUnitTests_Header

//CPPUnit Test Includes
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>


// Class of several tests
class SampleUnitTest : public CppUnit::TestCase
{
public:
    SampleUnitTest();
    
    // Actual test case functions and code
    void testObviousOne();
    
    void testObviousTwo();
    
    void testObviousThree();
    
    void testObviousFour();
    
    // Within the test class, a static method which loads all tests into an aggregate test object
    static CppUnit::Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Sample Unit Test");
        suiteOfTests->addTest(new CppUnit::TestCaller<SampleUnitTest>("testObviousOne",   &SampleUnitTest::testObviousOne));
        suiteOfTests->addTest(new CppUnit::TestCaller<SampleUnitTest>("testObviousTwo",   &SampleUnitTest::testObviousTwo));
        suiteOfTests->addTest(new CppUnit::TestCaller<SampleUnitTest>("testObviousThree", &SampleUnitTest::testObviousThree));
        suiteOfTests->addTest(new CppUnit::TestCaller<SampleUnitTest>("testObviousFour",  &SampleUnitTest::testObviousFour));
        return suiteOfTests;
    }
};


#endif
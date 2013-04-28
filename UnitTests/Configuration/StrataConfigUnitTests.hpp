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

#include "../../Source/Configuration/StrataConfig.hpp"


// Class of several tests
class StrataConfigUnitTests : public CppUnit::TestCase
{
public:
    StrataConfigUnitTests();
    
    
    //!Main Prototype header
    /*!  Better desc
     *  \pre
     *  \post
     *  \note NA*/
    void noConfigurationFile();
    
    
    //!Main Prototype header
    /*!  Better desc
     *  \pre
     *  \post
     *  \note NA*/
    void testObviousOne();
    
    // Within the test class, a static method which loads all tests into an aggregate test object
    static CppUnit::Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("StrataExtract Configuration Tests");
        suiteOfTests->addTest(new CppUnit::TestCaller<StrataConfigUnitTests>("testObviousOne",   &StrataConfigUnitTests::testObviousOne));
        suiteOfTests->addTest(new CppUnit::TestCaller<StrataConfigUnitTests>("testObviousOne",   &StrataConfigUnitTests::noConfigurationFile));
        return suiteOfTests;
    }
protected:
    
private:
    StrataConfig *myConfig;
};


#endif
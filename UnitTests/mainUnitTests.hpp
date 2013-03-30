/*!
 *  \brief
 *  \details
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      March 24, 2013
 *  \copyright GPLv2
 */

#ifndef Main_UnitTests_Header
#define Main_UnitTests_Header

//CPPUnit Test Includes
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include <boost/filesystem.hpp>

#include "./Configuration/StrataConfigUnitTests.hpp"

//Temporary Testing
#include "../Source/Archives/MPQ/MPQArchive.hpp"
#include "../Source/Conversion/Image/ImageMagick/MagickImage.hpp"
#include "../Source/Conversion/Audio/FFmpeg/AudioFFmpeg.hpp"
#include "../Source/Conversion/Video/FFmpeg/VideoFFmpeg.hpp"
#include "../Source/Configuration/StrataConfig.hpp"
#include "../Source/AssetManager/StrataRunner.hpp"

// Class of several tests
class MiscTests : public CppUnit::TestCase
{
public:
    MiscTests();
    
    // Actual test case functions and code
    void test1();
    
    // Within the test class, a static method which loads all tests into an aggregate test object
    static CppUnit::Test *suite()
    {
        CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Sample Unit Test");
        suiteOfTests->addTest(new CppUnit::TestCaller<MiscTests>("MiscOne",   &MiscTests::test1));
        return suiteOfTests;
    }
};

#endif
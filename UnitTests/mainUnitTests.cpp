#include "mainUnitTests.hpp"

int main()
{
	std::cout << "StrataExtract Core Unit Tests\n";
    
    CppUnit::TextUi::TestRunner runner;
    
    //Main Object
    runner.addTest( SampleUnitTest::suite() );
    
    runner.run();
	return 0;
}

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE StrataExtractTests

#include "mainUnitTests.hpp"
#include <boost/test/unit_test.hpp>

#include <iostream>



BOOST_AUTO_TEST_SUITE(MiscTests)

BOOST_AUTO_TEST_CASE(SetLogger)
{
    //boost::log::core::get()->set_filter(boost::log::trivial::severity > boost::log::trivial::trace);
}

/*BOOST_AUTO_TEST_CASE( Play )
{
    std::cout << "StrataConfig Tests\n";
    
    

    
    
    std::cout << "Done!\n";
}*/

BOOST_AUTO_TEST_SUITE_END()

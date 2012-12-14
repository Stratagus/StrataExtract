#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <Magick++.h>


void ConvertImage(boost::filesystem::path *sourceFilePath, boost::filesystem::path *destinationFilePath)
{
    Magick::Image *rawImage = new Magick::Image;
    std::cout << "ConvertImage Called by: " << boost::this_thread::get_id() << '\n';
    
    rawImage->read(sourceFilePath->string());
    rawImage->write(destinationFilePath->string());
    std::cout << "ConvertImage Complete by: " << boost::this_thread::get_id() << '\n';
}

void workerFunc()
{
    boost::posix_time::seconds workTime(1);
    
    std::cout << "Worker: running\n" << std::endl;
    
    // Pretend to do something useful...
    boost::this_thread::sleep(workTime);
    std::cout << "This thread is " << boost::this_thread::get_id() << '\n';
    
    std::cout << "Worker: finished\n" << std::endl;
}

int main()
{
	std::cout << "Boost Thread Example\n";
    boost::filesystem::path *sourceFilePath1 = new boost::filesystem::path;
    boost::filesystem::path *destinationFilePath1 = new boost::filesystem::path;
    *sourceFilePath1 = "/Users/brad/Desktop/source.png";
    *destinationFilePath1 = "/Users/brad/Desktop/dest.jpeg";
    boost::thread threadWork(workerFunc);
    boost::thread otherThread(ConvertImage, sourceFilePath1, destinationFilePath1);
    
    std::cout << "main:waiting for thread return\n";
    
    
    otherThread.join();
    threadWork.join();
    
    delete sourceFilePath1;
    delete destinationFilePath1;
    sourceFilePath1 = NULL;
    destinationFilePath1 = NULL;

	return 0;
}

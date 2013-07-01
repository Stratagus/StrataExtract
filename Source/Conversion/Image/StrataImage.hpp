#ifndef StrataImage_Header
#define StrataImage_Header

#include "StrataImageException.hpp"

#include <boost/filesystem/path.hpp>



class StrataImage
{
    public:
        virtual void ReadImageFromFile(boost::filesystem::path const &sourceImagePath) = 0;
        virtual void ReadImageFromMemory(std::vector<char> *image) = 0;
        virtual void WriteImageToFile(boost::filesystem::path const &destinationImagePath) = 0;
        virtual void ConvertImageFile(boost::filesystem::path const &sourceImagePath, boost::filesystem::path const &destinationimagePath) = 0;
    
    protected:
        StrataImage();
    private:
};

#endif
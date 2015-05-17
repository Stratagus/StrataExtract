#pragma once

#include "StrataImageException.hpp"

#include <boost/filesystem/path.hpp>

//There is an order of which to call each function, though some
//functions are not required for image operation

class StrataImage
{
    public:
        virtual void ReadImage(boost::filesystem::path const &sourceImagePath) = 0;
        virtual void ReadImage(std::vector<char> *image) = 0;
        virtual void WriteImageToFile(boost::filesystem::path const &destinationImagePath) = 0;
        virtual void ConvertImageFile(boost::filesystem::path const &sourceImagePath, boost::filesystem::path const &destinationimagePath) = 0;
    
        virtual void SetColorpalette(boost::filesystem::path colorPaletteFilePath) = 0;
        virtual void SetColorpalette(std::vector<char> *image) = 0;
    
    protected:
        StrataImage();
    private:
};
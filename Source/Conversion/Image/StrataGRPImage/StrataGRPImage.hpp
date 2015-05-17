#pragma once

#include "../StrataImage.hpp"
#include <boost/filesystem/path.hpp>
#include <libgrp.hpp>

class StrataGRPImage : public StrataImage
{
    public:
        StrataGRPImage();
        ~StrataGRPImage();
        virtual void ReadImage(boost::filesystem::path const &sourceImagePath) = 0;
        virtual void ReadImage(std::vector<char> *image) = 0;
        virtual void WriteImageToFile(boost::filesystem::path const &destinationImagePath) = 0;
        virtual void ConvertImageFile(boost::filesystem::path const &sourceImagePath, boost::filesystem::path const &destinationimagePath) = 0;
    
    //Only needed for GRPImages
    virtual void SetColorpalette(boost::filesystem::path colorPaletteFilePath) = 0;
    virtual void SetColorpalette(std::vector<char> *image) = 0;
    protected:
    ColorPalette *currentColorPalette;
    GRPImage *currentGRPImage;
    private:
};
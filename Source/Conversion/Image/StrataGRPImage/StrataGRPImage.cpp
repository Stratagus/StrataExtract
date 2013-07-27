#include "StrataGRPImage.hpp"
StrataGRPImage::StrataGRPImage()
{
    currentColorPalette = NULL;
    currentGRPImage = NULL;
}
StrataGRPImage::~StrataGRPImage()
{
    if(currentGRPImage)
    {
        delete currentGRPImage;
        currentGRPImage = NULL;
    }
    if(currentColorPalette)
    {
        delete currentColorPalette;
        currentColorPalette = NULL;
    }
}

void StrataGRPImage::ReadImage(boost::filesystem::path const &sourceImagePath)
{
    if(!currentGRPImage)
    {
        currentGRPImage = new GRPImage(sourceImagePath.string());
    }
    else
    {
        currentGRPImage->LoadImage(sourceImagePath.string());
    }
}

void StrataGRPImage::ReadImage(std::vector<char> *image)
{
    if(!currentGRPImage)
    {
        currentGRPImage = new GRPImage(image);
    }
    else
    {
        currentGRPImage->LoadImage(image);
    }
}

void StrataGRPImage::WriteImageToFile(boost::filesystem::path const &destinationImagePath)
{
    if(currentGRPImage)
    {
        currentGRPImage->SaveConvertedImage(destinationImagePath.string(), 0, currentGRPImage->getNumberOfFrames());
    }
    else
    {
        //throw something
    }
}

void StrataGRPImage::SetColorpalette(boost::filesystem::path colorPaletteFilePath)
{
    if(!currentColorPalette)
    {
        currentColorPalette = new ColorPalette;
    }
    currentColorPalette->LoadPalette(colorPaletteFilePath.string());
}

void StrataGRPImage::SetColorpalette(std::vector<char> *image)
{
    if(!currentColorPalette)
    {
        currentColorPalette = new ColorPalette;
    }
    currentColorPalette->LoadPalette(image);
}
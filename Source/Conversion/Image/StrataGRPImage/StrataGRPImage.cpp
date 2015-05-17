#include "StrataGRPImage.hpp"
StrataGRPImage::StrataGRPImage()
{

}
StrataGRPImage::~StrataGRPImage()
{

}

void StrataGRPImage::ReadImage(boost::filesystem::path const &sourceImagePath)
{
    if(!currentGRPImage)
        currentGRPImage = std::make_unique<GRPImage>(sourceImagePath.string());
    else
        currentGRPImage->LoadImage(sourceImagePath.string());
}

void StrataGRPImage::ReadImage(std::vector<char> *image)
{
    if(!currentGRPImage)
        currentGRPImage = std::make_unique<GRPImage>(image);
    else
        currentGRPImage->LoadImage(image);
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
        currentColorPalette = std::make_unique<ColorPalette>();

    currentColorPalette->LoadPalette(colorPaletteFilePath.string());
}

void StrataGRPImage::SetColorpalette(std::vector<char> *image)
{
    if(!currentColorPalette)
        currentColorPalette = std::make_unique<ColorPalette>();

    currentColorPalette->LoadPalette(image);
}
#include "MagickImage.hpp"
MagickImage::MagickImage()
{

}
MagickImage::~MagickImage()
{

}

void MagickImage::ReadImage(boost::filesystem::path const &sourceImagePath)
{
    if(!graphicImage)
        graphicImage = std::make_unique<Magick::Image>();
    
    graphicImage->read(sourceImagePath.string());
}

void MagickImage::ReadImage(std::vector<char> *image)
{
    Magick::Blob Blob;
    Blob.update(&image->at(0), image->size());
    
    if(!graphicImage)
        graphicImage = std::make_unique<Magick::Image>();
        
    graphicImage->read(Blob);
}

void MagickImage::WriteImageToFile(boost::filesystem::path const &destinationImagePath)
{
    if(!graphicImage)
    {
        throw "graphicImage not loaded before writing.";
    }
    else
        graphicImage->write(destinationImagePath.string());
}
void MagickImage::ConvertImageFile(boost::filesystem::path const &sourceImagePath, boost::filesystem::path const &destinationimagePath)
{
    if(!graphicImage)
        graphicImage = std::make_unique<Magick::Image>();

    graphicImage->read(sourceImagePath.string());
    graphicImage->write(destinationimagePath.string());
}
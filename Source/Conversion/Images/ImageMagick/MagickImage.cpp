#include "MagickImage.hpp"
MagickImage::MagickImage()
{
    graphicImage = NULL;
}
MagickImage::~MagickImage()
{
    delete graphicImage;
    graphicImage = NULL;
}
void MagickImage::ReadImageFromFile(boost::filesystem::path imagePath)
{
    if(graphicImage == NULL)
    {
        graphicImage = new Magick::Image;
    }
    graphicImage->read(imagePath.string());
}
void MagickImage::ReadImageFromMemory(std::vector<char> *image)
{
    Magick::Blob Blob;
    Blob.update(&image->at(0), image->size());
    if(graphicImage == NULL)
    {
        graphicImage = new Magick::Image;
    }
    graphicImage->read(Blob);
}
void MagickImage::WriteImageToFile(boost::filesystem::path destinationPath)
{
    if(!graphicImage)
    {
        throw "graphicImage not loaded before writing.";
    }
    else
        graphicImage->write(destinationPath.string());
}
void MagickImage::ConvertImageFile(boost::filesystem::path sourcePath, boost::filesystem::path destinationPath)
{
    if(graphicImage == NULL)
    {
        graphicImage = new Magick::Image;
    }
    graphicImage->read(sourcePath.string());
    graphicImage->write(sourcePath.string());
}
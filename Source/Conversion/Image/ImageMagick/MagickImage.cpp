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

void MagickImage::ReadImage(boost::filesystem::path const &sourceImagePath)
{
    if(graphicImage == NULL)
    {
        graphicImage = new Magick::Image;
    }
    graphicImage->read(sourceImagePath.string());
}

void MagickImage::ReadImage(std::vector<char> *image)
{
    Magick::Blob Blob;
    Blob.update(&image->at(0), image->size());
    if(graphicImage == NULL)
    {
        graphicImage = new Magick::Image;
    }
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
    if(graphicImage == NULL)
    {
        graphicImage = new Magick::Image;
    }
    graphicImage->read(sourceImagePath.string());
    graphicImage->write(destinationimagePath.string());
}
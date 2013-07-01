#ifndef GRPImage_Header
#define GRPImage_Header

#include "../StrataImage.hpp"
#include <boost/filesystem/path.hpp>

class GRPImage : public StrataImage
{
    public:
        GRPImage();
        ~GRPImage();
        //virtual void ReadImageFromFile(boost::filesystem::path const &sourceImagePath);
        //virtual void ReadImageFromMemory(std::vector<char> *image);
        //virtual void WriteImageToFile(boost::filesystem::path const &destinationImagePath);
        //virtual void ConvertImageFile(boost::filesystem::path const &sourceImagePath, boost::filesystem::path  const &destinationimagePath);
    protected:
    private:
};

#endif
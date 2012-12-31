#ifndef MagickImage_Header
#define MagickImage_Header

#include <Magick++.h>
#include <boost/filesystem/path.hpp>

class MagickImage
{
    public:
        //!MagickImage Constructor
        /*!  MagickImage Object constructor
         *  \pre NA
         *  \post MagickImage object is created and internal
         *        data structures set to NULL.
         *  \note NA*/
        MagickImage();
        ~MagickImage();
        void ReadImageFromFile(boost::filesystem::path imagePath);
        void ReadImageFromMemory(std::vector<char> *image);
        void WriteImageToFile(boost::filesystem::path destinationPath);
        void ConvertImageFile(boost::filesystem::path sourcePath, boost::filesystem::path destinationPath);
    
    
    protected:
        Magick::Image *graphicImage;
    
    private:
};

#endif
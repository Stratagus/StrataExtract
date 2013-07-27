#ifndef MagickImage_Header
#define MagickImage_Header

#include "../StrataImage.hpp"
#include <Magick++.h>


class MagickImage : public StrataImage
{
public:
    //!MagickImage Constructor
    /*!  MagickImage Object constructor
     *  \pre NA
     *  \post MagickImage object is created and internal
     *        data structures set to NULL.
     *  \note NA*/
    MagickImage();
    //!MagickImage Deconstructor
    /*!  MagickImage Object constructor
     *  \pre NA
     *  \post MagickImage object is created and internal
     *        data structures set to NULL.
     *  \note NA*/
    ~MagickImage();
    //!Read image from file to memory
    /*!  MagickImage Object constructor
     *  \pre NA
     *  \post MagickImage object is created and internal
     *        data structures set to NULL.
     *  \note NA*/
    virtual void ReadImage(boost::filesystem::path const &sourceImagePath);
    //!MagickImage Constructor
    /*!  MagickImage Object constructor
     *  \pre NA
     *  \post MagickImage object is created and internal
     *        data structures set to NULL.
     *  \note NA*/
    virtual void ReadImage(std::vector<char> *image);
    //!MagickImage Constructor
    /*!  MagickImage Object constructor
     *  \pre NA
     *  \post MagickImage object is created and internal
     *        data structures set to NULL.
     *  \note NA*/
    virtual void WriteImageToFile(boost::filesystem::path const &destinationImagePath);
    //!MagickImage Constructor
    /*!  MagickImage Object constructor
     *  \pre NA
     *  \post MagickImage object is created and internal
     *        data structures set to NULL.
     *  \note NA*/
    virtual void ConvertImageFile(boost::filesystem::path const &sourceImagePath, boost::filesystem::path  const &destinationimagePath);

protected:
    Magick::Image *graphicImage;
private:
};

#endif
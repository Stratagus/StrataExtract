#ifndef VideoLibav_Header
#define VideoLibav_Header

#include <boost/filesystem/path.hpp>
#include <string>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}


class VideoLibav
{
    public:
        VideoLibav();
        ~VideoLibav();
        void ConvertVideo(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath);
    
    protected:
    private:
};

#endif
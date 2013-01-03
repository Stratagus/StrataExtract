#ifndef VideoLibav_Header
#define VideoLibav_Header

#include <boost/filesystem/path.hpp>
#include <string>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/common.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/samplefmt.h>
}

#define FF_INPUT_BUFFER_PADDING_SIZE 16
#define INBUF_SIZE 4096

class VideoLibav
{
    public:
        VideoLibav();
        ~VideoLibav();
        void ConvertVideo(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath);
    void video_decode_example(const char *outfilename, const char *filename);
    void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

    protected:
    private:
};

#endif
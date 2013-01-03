#ifndef AudioLibav_Header
#define AudioLibav_Header

#include <boost/filesystem/path.hpp>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}
//These need to go away
#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096


class AudioLibav
{
    public:
    AudioLibav();
    ~AudioLibav();
    void ConvertAudio(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath);
    void DecodeAudio(std::vector<char> *audio);
    
    void audio_encode_example(const char *filename);
    void audio_decode_example(const char *outfilename, const char *filename);
    

    protected:
    private:
};
#endif
#ifndef AudioLibav_Header
#define AudioLibav_Header

#include <boost/filesystem/path.hpp>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>   
}

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

#define FORMAT_SIZE  30720
#define SAMPLE_SIZE 6144


class AudioLibav
{
    public:
    AudioLibav();
    ~AudioLibav();
    void ConvertAudio(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath);
    void EncodeAudio();
    void DecodeAudio(std::vector<char> *inputAudio);
    
    void audio_encode_example(const char *filename);
    
    int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt);
    int select_sample_rate(AVCodec *codec);
    int select_channel_layout(AVCodec *codec);
    
    #warning Change to protected
    std::vector<char> *audio;

    protected:
    
    
    private:
};
#endif
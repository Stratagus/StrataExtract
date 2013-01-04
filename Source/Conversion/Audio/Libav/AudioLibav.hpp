#ifndef AudioLibav_Header
#define AudioLibav_Header

#include <boost/filesystem/path.hpp>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h> 
    #include <libswresample/swresample.h>
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
    
    int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt);
    int select_sample_rate(AVCodec *codec);
    int select_channel_layout(AVCodec *codec);
    bool isEncoded();


    protected:
    AVSampleFormat Resample(AVCodec *avCodec);
    
    int audioSampleRate;
    int audioBitRate;
    int audioBitsPerSample;
    int audioChannels;
    
    bool audioEncoded;
    std::vector<char> *audio;
    enum AVSampleFormat audioSampleFormat;
    
    private:
};
#endif
#include "AudioLibav.hpp"
AudioLibav::AudioLibav()
{
    audio = NULL;
}
AudioLibav::~AudioLibav()
{
    delete audio;
    audio = NULL;
}
void AudioLibav::ConvertAudio(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath)
{
    //Defered from pattern here
    //std::cout <<
    //outputBuffer = reinterpret_cast<uint8_t *>(malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE));
}

void AudioLibav::DecodeAudio(std::vector<char> *inputAudio)
{
    
    //Lazy instantiation
    if(audio == NULL)
    {
        audio = new std::vector<char>;
    }
    else
    {
        delete audio;
        audio = new std::vector<char>;
    }
    //Wasteful?
    avcodec_register_all();
    
    unsigned int inputAudioVectorPosition = 0;
    unsigned int audioVectorPosition = 0;
    
    AVCodec *codec;
    AVCodecContext *codecContext = NULL;
    int length;
    uint8_t audioBuffer[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    AVPacket audioPacket;
    AVFrame *decodedFrame = NULL;
    
    av_init_packet(&audioPacket);
    
    printf("Audio decoding\n");
    
    /* find the mpeg audio decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_PCM_S16LE);
    if (!codec)
    {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    codecContext = avcodec_alloc_context3(codec);
    
    #warning Bad assumption!!
    codecContext->channels = 2;
    
    /* open it */
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    /* decode until eof */
    audioPacket.data = audioBuffer;
    audioPacket.size = AUDIO_INBUF_SIZE;
    memcpy(audioBuffer, &inputAudio->at(0), AUDIO_INBUF_SIZE);
    
    inputAudioVectorPosition = AUDIO_INBUF_SIZE;

    while (audioPacket.size > 0)
    {
        int got_frame_mem = 0;
        
        if(!decodedFrame)
        {
            if (!(decodedFrame = avcodec_alloc_frame()))
            {
                fprintf(stderr, "out of memory\n");
                exit(1);
            }
        }
        else
        {
                        avcodec_get_frame_defaults(decodedFrame);
        }

        length = avcodec_decode_audio4(codecContext, decodedFrame, &got_frame_mem, &audioPacket);
        
        if (length < 0)
        {
            fprintf(stderr, "Error while decoding\n");
            exit(1);
        }
        if(got_frame_mem )
        {
            int data_size_mem = av_samples_get_buffer_size(NULL, codecContext->channels, decodedFrame->nb_samples, codecContext->sample_fmt, 1);
            
            
            
            audio->resize((audio->size() + data_size_mem));
            memcpy(&audio->at(audioVectorPosition), decodedFrame->data[0], data_size_mem);
            audioVectorPosition += data_size_mem;

            //Write output to file
            //fwrite(decoded_frame_mem->data[0], 1, data_size_mem, memoutfile);
        }
        
        audioPacket.size -= length;
        audioPacket.data += length;
        
        if(audioPacket.size < AUDIO_REFILL_THRESH)
        {
            memmove(audioBuffer, audioPacket.data, audioPacket.size);
            audioPacket.data = audioBuffer;
            //std::cout << "Mempacket data: " << memPkt.data << "   MemPacket size: " << memPkt.size << "    memAUDIO BUFF - size: " << AUDIO_INBUF_SIZE - memPkt.size;
            if((inputAudio->size() - inputAudioVectorPosition) < (AUDIO_INBUF_SIZE - audioPacket.size))
            {
                memcpy( (audioPacket.data + audioPacket.size) , &inputAudio->at(inputAudioVectorPosition), (inputAudio->size() - inputAudioVectorPosition));
                inputAudioVectorPosition = inputAudio->size() - 1;
                #warning Bad Assumption?
                //memlen = AUDIO_INBUF_SIZE - memPkt.size;
                length = 0;
            }
            else
            {
                memcpy( (audioPacket.data + audioPacket.size) , &inputAudio->at(inputAudioVectorPosition), AUDIO_INBUF_SIZE - audioPacket.size);
                inputAudioVectorPosition = inputAudioVectorPosition + (AUDIO_INBUF_SIZE - audioPacket.size);
                length = AUDIO_INBUF_SIZE - audioPacket.size;
            }
            
            if(length > 0)
            {
                audioPacket.size += length;
            }
        }
    }
    
    avcodec_close(codecContext);
    av_free(codecContext);
    avcodec_free_frame(&decodedFrame);

}
void AudioLibav::EncodeAudio()
{
    if(!audio)
    {
        throw "No audio has been loaded & decoded";
    }
    
    #warning Wasteful?
    av_register_all();
    
    AVCodec *codec;
    AVCodecContext *codecContext = NULL;
    AVFrame *encodedFrame;
    AVPacket audioPacket;
    int i, j, k, ret, got_output;
    int bufferSize;
    FILE *file;
    uint16_t *samples;
    float t, tincr;
    
    std::cout << "Audio Encoding" << '\n';
    
    codec = avcodec_find_encoder(AV_CODEC_ID_VORBIS);
    if(!codec)
    {
        throw "codec not found";
    }
    
    codecContext = avcodec_alloc_context3(codec);
    
    
    /* open it */
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
}



/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
{
    const enum AVSampleFormat *p = codec->sample_fmts;
    
    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == sample_fmt)
            return 1;
        p++;
    }
    return 0;
}

/* just pick the highest supported samplerate */
static int select_sample_rate(AVCodec *codec)
{
    const int *p;
    int best_samplerate = 0;
    
    if (!codec->supported_samplerates)
        return 44100;
    
    p = codec->supported_samplerates;
    while (*p) {
        best_samplerate = FFMAX(*p, best_samplerate);
        p++;
    }
    return best_samplerate;
}

/* select layout with the highest channel count */
static int select_channel_layout(AVCodec *codec)
{
    const uint64_t *p;
    uint64_t best_ch_layout = 0;
    int best_nb_channells   = 0;
    
    if (!codec->channel_layouts)
        return AV_CH_LAYOUT_STEREO;
    
    p = codec->channel_layouts;
    while (*p) {
        int nb_channels = av_get_channel_layout_nb_channels(*p);
        
        if (nb_channels > best_nb_channells) {
            best_ch_layout    = *p;
            best_nb_channells = nb_channels;
        }
        p++;
    }
    return best_ch_layout;
}

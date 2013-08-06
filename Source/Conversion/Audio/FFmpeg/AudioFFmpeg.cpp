#include "AudioFFmpeg.hpp"

AudioFFmpeg::AudioFFmpeg()
{
    audio = NULL;
    audioAttributes = NULL;
    audioEncoded = false;
}
AudioFFmpeg::~AudioFFmpeg()
{
    delete audio;
    audio = NULL;
    if(audioAttributes)
        delete audioAttributes;
    audioAttributes = NULL;
}
void AudioFFmpeg::ConvertAudio(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath)
{
    //Defered from pattern here
    //std::cout <<
    //outputBuffer = reinterpret_cast<uint8_t *>(malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE));
}


#warning !!!NOT OPTIMIZED AT ALL!!!
void AudioFFmpeg::DecodeAudio(std::vector<char> *inputAudio)
{
    
    //#warning Writeout
    FILE* outfile = fopen("testraw", "wb");
    
    unsigned int inputAudioVectorPosition = 0;
    unsigned int audioVectorPosition = 0;
    
    AVCodec *codec;
    AVCodecContext *codecContext = NULL;
    int length;
    AVPacket audioPacket;
    AVFrame *decodedFrame = NULL;
    
    av_init_packet(&audioPacket);
    
    //Lazy instantiation
    if(!audio)
    {
        audio = new std::vector<char>;
    }
    else
    {
        delete audio;
        audio = new std::vector<char>;
    }
    if(!audioAttributes)
    {
        audioAttributes = avformat_alloc_context();
    }
    else
    {
        avformat_free_context(audioAttributes);
        audioAttributes = avformat_alloc_context();
    }
    #warning Wasteful?
    av_register_all();
    

    //Attach a pointer from the audioAttributes to the inputAudio
    audioAttributes->pb = avio_alloc_context((unsigned char *) &inputAudio->front(), inputAudio->size(), 0, NULL, NULL, NULL, NULL);
    
    
    

    if (avformat_open_input(&audioAttributes, "", NULL, NULL) < 0)
    {
        throw "Could not open the audioInput in audioAttributes";
    }
    
    if(audioAttributes->nb_streams > 1)
    {
        throw "Cannot support more then one stream yet";
    }
    
    //Output the audioAttributes found
    av_dump_format(audioAttributes, 0, 0, 0);
    
    //std::cout << "Codec: " << audioAttributes->streams[0]->codec << '\n';
    //std::cout << "Sample Rate: " << audioAttributes->streams[0]->codec->sample_rate << '\n';
    //std::cout << "Sample Format: " << audioAttributes->streams[0]->codec->sample_fmt << '\n';
    //std::cout<< "Number of Frames: " << audioAttributes->streams[0]->codec_info_nb_frames << '\n';
    //std::cout<< "Duration: " << audioAttributes->streams[0]->duration << '\n';
    
    
    printf("Audio decoding\n");
    
    codec = avcodec_find_decoder(audioAttributes->streams[0]->codec->codec_id);
    if (!codec)
    {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    codecContext = avcodec_alloc_context3(codec);
    
    codecContext->channels = audioAttributes->streams[0]->codec->channels;
    
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    audioPacket.data = (uint8_t *) &inputAudio->front();
    audioPacket.size = inputAudio->size();
    //memcpy(audioBuffer, &inputAudio->at(0), AUDIO_INBUF_SIZE);
    
    inputAudioVectorPosition = AUDIO_INBUF_SIZE;

    while (audioPacket.size > 0)
    {
        std::cout << "Packet Size: " << audioPacket.size << '\n';
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
            fwrite(decodedFrame->data[0], 1, data_size_mem, outfile);
        }
        
        audioPacket.size -= length;
        audioPacket.data += length;
        
        /*if(audioPacket.size < AUDIO_REFILL_THRESH)
        {
            memmove(audioBuffer, audioPacket.data, audioPacket.size);
            audioPacket.data = audioBuffer;
            //std::cout << "Mempacket data: " << memPkt.data << "   MemPacket size: " << memPkt.size << "    memAUDIO BUFF - size: " << AUDIO_INBUF_SIZE - memPkt.size;
            
            std::cout << "Incoming Size: " << inputAudio->size() << " InputVectorPos: " << inputAudioVectorPosition << '\n';
            if((inputAudio->size() - inputAudioVectorPosition) < (AUDIO_INBUF_SIZE - audioPacket.size))
            {
                memcpy( (audioPacket.data + audioPacket.size) , &inputAudio->at(inputAudioVectorPosition), (inputAudio->size() - inputAudioVectorPosition));
                length = ((inputAudio->size() - 1) - inputAudioVectorPosition) - audioPacket.size;
                inputAudioVectorPosition = inputAudio->size() - 1;
                #warning Bad Assumption?
                if(audioPacket.size == 1)
                {
                    break;
                }
                
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
        }*/
    }
    
    avcodec_close(codecContext);
    av_free(codecContext);
    avcodec_free_frame(&decodedFrame);

}
void AudioFFmpeg::EncodeAudio()
{
    if(!audio)
    {
        throw "no audio decoded";
    }
    
    #warning Wasteful?
    avcodec_register_all();
    
    AVCodec *codec;
    AVCodecContext *c= NULL;
    AVFrame *frame;
    AVPacket pkt;
    int ret, got_output;
    int buffer_size;
    FILE *f;
    int vectorPosition = audioAttributes->pb->buf_ptr - audioAttributes->pb->buffer ;
    
    printf("Audio encoding\n");
    
    //find the MP2 encoder 
    codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    
    c = avcodec_alloc_context3(codec);
    
    // put sample parameters 
    c->bit_rate = 128000;
    
    
    // check that the encoder supports s16 pcm input 
    
    c->sample_fmt = AV_SAMPLE_FMT_S16;
    if (!check_sample_fmt(codec, c->sample_fmt))
    {
        fprintf(stderr, "encoder does not support %s Resampling..\n", av_get_sample_fmt_name(c->sample_fmt));
        //c->sample_fmt = Resample(codec, c);
        exit(-1);
    }
    
    ///select other audio parameters supported by the encoder 
    #warning use audioSampleRate instead
    c->sample_rate = 22050;
    std::cout << "Channel Layout: "  << audioAttributes->streams[0]->codec->channel_layout << " Should be: " << AV_CH_LAYOUT_MONO << '\n';
    c->channel_layout = AV_CH_LAYOUT_MONO;
    c->channels       = av_get_channel_layout_nb_channels(audioAttributes->streams[0]->codec->channels);
    c->bits_per_raw_sample = 16;
    
    // open it 
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    f = fopen("encoded.mp2", "wb");
    if (!f) {
        //fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }
    
    // frame containing input raw audio 
    frame = avcodec_alloc_frame();
    if (!frame)
    {
        fprintf(stderr, "could not allocate audio frame\n");
        exit(1);
    }
    
    frame->nb_samples     = c->frame_size;
    frame->format         = c->sample_fmt;
    frame->channel_layout = c->channel_layout;
    
    //the codec gives us the frame size, in samples,
     // we calculate the size of the samples buffer in bytes
    buffer_size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size, c->sample_fmt, 0);
    
    /*samples = reinterpret_cast<uint8_t *>(av_malloc(buffer_size));
    if (!samples) {
        fprintf(stderr, "could not allocate %d bytes for samples buffer\n",
                buffer_size);
        exit(1);
    }*/

    
    //Get the data and encode
    while (vectorPosition < audio->size() - buffer_size)
    {
        av_init_packet(&pkt);
        pkt.data = NULL; // packet data will be allocated by the encoder
        pkt.size = 0;

        ret = avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt, (const uint8_t*) &audio->at(vectorPosition), audio->size(), 0);

        buffer_size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size, c->sample_fmt, 0);
        // encode the samples 
        ret = avcodec_encode_audio2(c, &pkt, frame, &got_output);
        if (ret < 0)
        {
            fprintf(stderr, "error encoding audio frame\n");
            exit(1);
        }
        
        if (got_output) {
            fwrite(pkt.data, 1, pkt.size, f);
            av_free_packet(&pkt);
        }
        
        vectorPosition += buffer_size;
    }
    
    fclose(f);
    
    //av_freep(&samples);
    avcodec_free_frame(&frame);
    avcodec_close(c);
    av_free(c);
    
    audioEncoded = true;
}

AVSampleFormat AudioFFmpeg::Resample(AVCodec *avCodec, AVCodecContext *avCodecContext)
{
    SwrContext *resampleContext = NULL;
    uint8_t **inputBuffer = NULL;
    uint8_t **outputBuffer= NULL;
    
    int ret, dst_nb_samples, max_dst_nb_samples;
    int sourceNumberofChannels;
    
    int audioVectorPosition = 0;
    int resampledAudioVectorPosition = 0;
    
    int numberOfPlanes;
    
    std::vector<char> *resampledAudio = new std::vector<char>;
    
    
    #warning Remove AV_SAMPLE_FMT_FLT to dynamic instead
    //resampleContext = swr_alloc_set_opts(resampleContext, audioChannelLayout,
      //                                   AV_SAMPLE_FMT_FLT, audioSampleRate,
        //                                 audioChannelLayout, audioSampleFormat,
          //                               audioSampleRate, 0, 0);
    if((ret = swr_init(resampleContext)) < 0)
    {
        std::cout << "Could not init resampler context\n";
        exit(-1);
    }
    
    //sourceNumberofChannels = av_get_channel_layout_nb_channels(audioChannelLayout);
    
    


}

//check that a given sample format is supported by the encoder
int AudioFFmpeg::check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
{
    const enum AVSampleFormat *p = codec->sample_fmts;
    
    while (*p != AV_SAMPLE_FMT_NONE)
    {
        
        if (*p == sample_fmt)
            return 1;
        p++;
    }
    return 0;
}





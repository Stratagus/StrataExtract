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

void AudioFFmpeg::DecodeAudio(std::vector<char> *inputAudio)
{
    //Register all the availible codecs in order to ensure that
    //we will try to find whatever codec is being used
    av_register_all();
    
    //If the debugger write out flag is enabled, write
    //the raw 
    #if AUDIODECODEWRITEOUT
        FILE* outfile = fopen("testraw", "wb");
    #endif
    
    //Use the following integers to keep track of our
    //current position on the vectors
    unsigned int inputAudioVectorPosition = 0;
    unsigned int audioVectorPosition = 0;
    
    AVCodec *codec;
    AVCodecContext *decoderProperties = NULL;
    int length;
    AVPacket audioPacket;
    AVFrame *decodedFrame = NULL;
    
    av_init_packet(&audioPacket);
    
    //Lazy instantiation
    if(!audio)
    {
        audio = new std::vector<uint8_t>;
    }
    else
    {
        delete audio;
        audio = new std::vector<uint8_t>;
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
    
    codec = avcodec_find_decoder(audioAttributes->streams[0]->codec->codec_id);
    if (!codec)
    {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    decoderProperties = avcodec_alloc_context3(codec);
    
    decoderProperties->channels = audioAttributes->streams[0]->codec->channels;
    
    if (avcodec_open2(decoderProperties, codec, NULL) < 0) {
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

        length = avcodec_decode_audio4(decoderProperties, decodedFrame, &got_frame_mem, &audioPacket);
        
        if (length < 0)
        {
            fprintf(stderr, "Error while decoding\n");
            exit(1);
        }
        if(got_frame_mem )
        {
            int data_size_mem = av_samples_get_buffer_size(NULL, decoderProperties->channels, decodedFrame->nb_samples, decoderProperties->sample_fmt, 1);
            
            
            audio->resize((audio->size() + data_size_mem));
            memcpy(&audio->at(audioVectorPosition), decodedFrame->data[0], data_size_mem);
            audioVectorPosition += data_size_mem;

            //If debug writeout is enabled write out the values to a file
            #if AUDIODECODEWRITEOUT
                fwrite(decodedFrame->data[0], 1, data_size_mem, outfile);
            #endif
        }
        
        audioPacket.size -= length;
        audioPacket.data += length;
    }
    
    avcodec_close(decoderProperties);
    av_free(decoderProperties);
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
    c->channels       = 1;
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





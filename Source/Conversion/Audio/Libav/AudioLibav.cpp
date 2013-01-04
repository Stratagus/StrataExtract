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
    #warning Wasteful?
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
    
    codec = avcodec_find_decoder(AV_CODEC_ID_PCM_S16LE);
    if (!codec)
    {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    codecContext = avcodec_alloc_context3(codec);
    
    #warning Bad assumption!!
    codecContext->channels = 2;
    
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
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
        throw "no audio decoded";
    }
    
    #warning Wasteful?
    avcodec_register_all();
    
    AVCodec *codec;
    AVCodecContext *c= NULL;
    AVFrame *frame;
    AVPacket pkt;
    int j, k, ret, got_output;
    int buffer_size;
    FILE *f;
    uint16_t *samples;
    float t, tincr;
    
    printf("Audio encoding\n");
    
    /* find the MP2 encoder */
    codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    
    c = avcodec_alloc_context3(codec);
    
    /* put sample parameters */
    c->bit_rate = 128000;
    
    /* check that the encoder supports s16 pcm input */
    c->sample_fmt = AV_SAMPLE_FMT_S16;
    if (!check_sample_fmt(codec, c->sample_fmt)) {
        fprintf(stderr, "encoder does not support %s",
                av_get_sample_fmt_name(c->sample_fmt));
        exit(1);
    }
    
    /* select other audio parameters supported by the encoder */
    //c->sample_rate    = select_sample_rate(codec);
    c->sample_rate = 22050;
    c->channel_layout = select_channel_layout(codec);
    c->channels       = av_get_channel_layout_nb_channels(c->channel_layout);
    
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    f = fopen("encoded.mp3", "wb");
    if (!f) {
        //fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }
    
    /* frame containing input raw audio */
    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "could not allocate audio frame\n");
        exit(1);
    }
    
    frame->nb_samples     = c->frame_size;
    frame->format         = c->sample_fmt;
    frame->channel_layout = c->channel_layout;
    
    /* the codec gives us the frame size, in samples,
     * we calculate the size of the samples buffer in bytes */
    buffer_size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size,
                                             c->sample_fmt, 0);
    samples = reinterpret_cast<uint16_t *>(av_malloc(buffer_size));
    if (!samples) {
        fprintf(stderr, "could not allocate %d bytes for samples buffer\n",
                buffer_size);
        exit(1);
    }
    /* setup the data pointers in the AVFrame */
    

    int vectorLocation = 0;
    //Get the data and encode
    while (vectorLocation < audio->size())
    {
        av_init_packet(&pkt);
        pkt.data = NULL; // packet data will be allocated by the encoder
        pkt.size = 0;
        
        if((audio->size() - vectorLocation) < buffer_size)
        {
            memcpy(samples, &audio->at(vectorLocation), (audio->size() - vectorLocation));
            vectorLocation += (audio->size() - vectorLocation);
        }
        else
        {
            memcpy(samples, &audio->at(vectorLocation), buffer_size);
            vectorLocation += buffer_size;
        }

        ret = avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt, (const uint8_t*)samples, buffer_size, 0);

        /* encode the samples */
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
    }
    fclose(f);
    
    av_freep(&samples);
    avcodec_free_frame(&frame);
    avcodec_close(c);
    av_free(c);
}



/* check that a given sample format is supported by the encoder */
int AudioLibav::check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
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

/* just pick the highest supported samplerate */
int AudioLibav::select_sample_rate(AVCodec *codec)
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
int AudioLibav::select_channel_layout(AVCodec *codec)
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

void AudioLibav::audio_encode_example(const char *filename)
{
    av_register_all();
    
    AVCodec *codec;
    AVCodecContext *c= NULL;
    AVFrame *frame;
    AVPacket pkt;
    int i, j, k, ret, got_output;
    int buffer_size;
    FILE *f;
    uint16_t *samples;
    float t, tincr;
    
    printf("Audio encoding\n");
    
    /* find the MP2 encoder */
    codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    
    c = avcodec_alloc_context3(codec);
    
    /* put sample parameters */
    c->bit_rate = 64000;
    
    /* check that the encoder supports s16 pcm input */
    c->sample_fmt = AV_SAMPLE_FMT_S16;
    if (!check_sample_fmt(codec, c->sample_fmt)) {
        fprintf(stderr, "encoder does not support %s",
                av_get_sample_fmt_name(c->sample_fmt));
        exit(1);
    }
    
    /* select other audio parameters supported by the encoder */
    c->sample_rate    = select_sample_rate(codec);
    c->channel_layout = select_channel_layout(codec);
    c->channels       = av_get_channel_layout_nb_channels(c->channel_layout);
    
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }
    
    /* frame containing input raw audio */
    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "could not allocate audio frame\n");
        exit(1);
    }
    
    frame->nb_samples     = c->frame_size;
    frame->format         = c->sample_fmt;
    frame->channel_layout = c->channel_layout;
    
    /* the codec gives us the frame size, in samples,
     * we calculate the size of the samples buffer in bytes */
    buffer_size = av_samples_get_buffer_size(NULL, c->channels, c->frame_size,
                                             c->sample_fmt, 0);
    samples = reinterpret_cast<uint16_t *>(av_malloc(buffer_size));
    if (!samples) {
        fprintf(stderr, "could not allocate %d bytes for samples buffer\n",
                buffer_size);
        exit(1);
    }
    /* setup the data pointers in the AVFrame */
    ret = avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt,
                                   (const uint8_t*)samples, buffer_size, 0);
    if (ret < 0) {
        fprintf(stderr, "could not setup audio frame\n");
        exit(1);
    }
    
    /* encode a single tone sound */
    t = 0;
    tincr = 2 * M_PI * 440.0 / c->sample_rate;
    for(i=0;i<200;i++) {
        av_init_packet(&pkt);
        pkt.data = NULL; // packet data will be allocated by the encoder
        pkt.size = 0;
        
        for (j = 0; j < c->frame_size; j++) {
            samples[2*j] = (int)(sin(t) * 10000);
            
            for (k = 1; k < c->channels; k++)
                samples[2*j + k] = samples[2*j];
            t += tincr;
        }
        /* encode the samples */
        ret = avcodec_encode_audio2(c, &pkt, frame, &got_output);
        if (ret < 0) {
            fprintf(stderr, "error encoding audio frame\n");
            exit(1);
        }
        if (got_output) {
            fwrite(pkt.data, 1, pkt.size, f);
            av_free_packet(&pkt);
        }
    }
    fclose(f);
    
    av_freep(&samples);
    avcodec_free_frame(&frame);
    avcodec_close(c);
    av_free(c);
}

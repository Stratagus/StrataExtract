#include "AudioLibav.hpp"
AudioLibav::AudioLibav()
{
    
}
AudioLibav::~AudioLibav()
{
    
}
void AudioLibav::ConvertAudio(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath)
{
    //Defered from pattern here
    //std::cout <<
    //outputBuffer = reinterpret_cast<uint8_t *>(malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE));
}

void AudioLibav::DecodeAudio(std::vector<char> *audio)
{
    //Wasteful?
    avcodec_register_all();
    
    
    int vectorPosition = 0;
    
    AVCodec *codec;
    AVCodec *memcodec;
    AVCodecContext *c= NULL;
    AVCodecContext *memc = NULL;
    int len, memlen;
    FILE *f, *outfile, *memoutfile;
    uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    uint8_t memBuffer[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    AVPacket avpkt, memPkt;
    AVFrame *decoded_frame = NULL;
    AVFrame *decoded_frame_mem = NULL;
    
    av_init_packet(&avpkt);
    av_init_packet(&memPkt);
    
    printf("Audio decoding\n");
    
    
    /* find the mpeg audio decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_PCM_S16LE);
    memcodec = avcodec_find_decoder(AV_CODEC_ID_PCM_S16LE);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    
    c = avcodec_alloc_context3(codec);
    memc = avcodec_alloc_context3(memcodec);
    
    
    //Bad assumption
    c->channels = 2;
    memc->channels = 2;
    
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    if (avcodec_open2(memc, memcodec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    f = fopen("test.wav", "rb");
    if (!f) {
        //fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }
    outfile = fopen("testraw", "wb");
    memoutfile = fopen("testrawmem", "wb");
    if (!outfile) {
        av_free(c);
        exit(1);
    }
    
    /* decode until eof */
    avpkt.data = inbuf;
    avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);
    
    memPkt.data = memBuffer;
    memPkt.size = AUDIO_INBUF_SIZE;
    memcpy(memBuffer, &audio->at(0), AUDIO_INBUF_SIZE);
    
    vectorPosition = AUDIO_INBUF_SIZE;
    
    std::cout << "AVPacket Data: " << avpkt.data << " AVPacket Size: " << avpkt.size << '\n'
              << "MemPacket Data: " << memPkt.data << " MemPacket Size: " << memPkt.size << '\n';
    
    
    for(int i = 0; i < AUDIO_INBUF_SIZE; i++)
    {
        if(memBuffer[i] != inbuf[i])
        {
            std::cout << "!!!!!NOT THE SAME at " << (uint8_t) i << "\n";
            std::cout << inbuf[i] << "  to  " << memBuffer[i];
            exit(-1);
        }
        //std::cout << "Data " << i << "    to    " << inbuf[i] << "  " << memBuffer[i] << '\n';
        //std::cout << "Data Position" << i << "  " << inbuf[i] << "  to  " << (uint8_t) audio->at(i) << '\n';
        
    }
    std::cout << "Input buffers are the same at start!!!\n";

    
    //while (avpkt.size > 0) {
    while (memPkt.size > 0 || avpkt.size > 0)
    {
        std::cout << "AVPacket Size: " << avpkt.size <<  "  Mempacket size: " << memPkt.size << '\n';
        int got_frame = 0;
        int got_frame_mem = 0;
        
        if(!decoded_frame_mem)
        {
            if (!(decoded_frame_mem = avcodec_alloc_frame()))
            {
                fprintf(stderr, "out of memory\n");
                exit(1);
            }
        }
        else
        {
                        avcodec_get_frame_defaults(decoded_frame_mem);
        }

        
        if (!decoded_frame)
        {
            if (!(decoded_frame = avcodec_alloc_frame()))
            {
                fprintf(stderr, "out of memory\n");
                exit(1);
            }
        }
        else
        {
                        avcodec_get_frame_defaults(decoded_frame);
        }

        
        //std::cout << "AVDECODE: " << c << "   " << decoded_frame << "  " << &got_frame << "  " << &avpkt << '\n';
        len = avcodec_decode_audio4(c, decoded_frame, &got_frame, &avpkt);
        //std::cout << "MEMDECODE: " << memc << "   " << decoded_frame_mem << "  " << &got_frame_mem << "  " << &memPkt << '\n';
        memlen = avcodec_decode_audio4(memc, decoded_frame_mem, &got_frame_mem, &memPkt);
        
        if (memlen < 0 || len < 0)
        {
            fprintf(stderr, "Error while decoding\n");
            exit(1);
        }
        if (got_frame)
        {
            //if a frame has been decoded, output it 
            int data_size = av_samples_get_buffer_size(NULL, c->channels, decoded_frame->nb_samples, c->sample_fmt, 1);
            fwrite(decoded_frame->data[0], 1, data_size, outfile);
        }
        if(got_frame_mem )
        {
            int data_size_mem = av_samples_get_buffer_size(NULL, memc->channels, decoded_frame_mem->nb_samples, memc->sample_fmt, 1);
            fwrite(decoded_frame_mem->data[0], 1, data_size_mem, memoutfile);
        }
        
        avpkt.size -= len;
        avpkt.data += len;
        memPkt.size -= memlen;
        memPkt.data += memlen;
        
        if (avpkt.size < AUDIO_REFILL_THRESH)
        {
            memmove(inbuf, avpkt.data, avpkt.size);
            avpkt.data = inbuf;
            std::cout << "Avpacket data: " << avpkt.data << "AVPacket size: " << avpkt.size << "AUDIO BUFF - size: " << AUDIO_INBUF_SIZE - avpkt.size;
            len = fread(avpkt.data + avpkt.size, 1, AUDIO_INBUF_SIZE - avpkt.size, f);
            std::cout << "  Length: " << len << '\n';
            if (len > 0)
                avpkt.size += len;
        }
        
        if(memPkt.size < AUDIO_REFILL_THRESH)
        {
            memmove(memBuffer, memPkt.data, memPkt.size);
            memPkt.data = memBuffer;
            std::cout << "Mempacket data: " << memPkt.data << "   MemPacket size: " << memPkt.size << "    memAUDIO BUFF - size: " << AUDIO_INBUF_SIZE - memPkt.size;
            if((audio->size() - vectorPosition) < (AUDIO_INBUF_SIZE - memPkt.size))
            {
                
                std::cout << "\n\n\n!!!!!!END of stream!!!!!\n\n\n";
                std::cout << "Size: " << audio->size() << "   Vector Position: " << vectorPosition << "  Length: " << memlen << "  Mempku size: " << memPkt.size << '\n';
                memcpy( (memPkt.data + memPkt.size) , &audio->at(vectorPosition), (audio->size() - vectorPosition));
                vectorPosition = audio->size() - 1;
                //memlen = AUDIO_INBUF_SIZE - memPkt.size;
                memlen = audio->size() - vectorPosition;
            }
            else
            {
                memcpy( (memPkt.data + memPkt.size) , &audio->at(vectorPosition), AUDIO_INBUF_SIZE - memPkt.size);
                vectorPosition = vectorPosition + (AUDIO_INBUF_SIZE - memPkt.size);
                memlen = AUDIO_INBUF_SIZE - memPkt.size;
            }

            std::cout << "   Vector Position: " << vectorPosition << "  Length: " << memlen << " Mempkt size: " << memPkt.size << '\n';
            
            if(memlen > 0)
            {
                memPkt.size += memlen;
            }
            
        }

    }
    
    fclose(outfile);
    fclose(f);
    
    avcodec_close(c);
    av_free(c);
    avcodec_free_frame(&decoded_frame);

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

void AudioLibav::audio_decode_example(const char *outfilename, const char *filename)
{
    //Wasteful?
    avcodec_register_all();
    
    
    AVCodec *codec;
    AVCodecContext *c= NULL;
    int len;
    FILE *f, *outfile;
    uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    AVPacket avpkt;
    AVFrame *decoded_frame = NULL;
    
    av_init_packet(&avpkt);
    
    printf("Audio decoding\n");

    
    /* find the mpeg audio decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_PCM_S16LE);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }
    
    c = avcodec_alloc_context3(codec);
    
    
    //Bad assumption
    c->channels = 2;
    
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }
    
    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }
    outfile = fopen(outfilename, "wb");
    if (!outfile) {
        av_free(c);
        exit(1);
    }
    
    /* decode until eof */
    avpkt.data = inbuf;
    avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);
    
    while (avpkt.size > 0) {
        
        std::cout << "AKPT size: " << avpkt.size << '\n';
        int got_frame = 0;
        
        if (!decoded_frame) {
            if (!(decoded_frame = avcodec_alloc_frame())) {
                fprintf(stderr, "out of memory\n");
                exit(1);
            }
        } else
            avcodec_get_frame_defaults(decoded_frame);
        
        len = avcodec_decode_audio4(c, decoded_frame, &got_frame, &avpkt);
        if (len < 0) {
            fprintf(stderr, "Error while decoding\n");
            exit(1);
        }
        if (got_frame) {
            /* if a frame has been decoded, output it */
            int data_size = av_samples_get_buffer_size(NULL, c->channels,
                                                       decoded_frame->nb_samples,
                                                       c->sample_fmt, 1);
            fwrite(decoded_frame->data[0], 1, data_size, outfile);
        }
        avpkt.size -= len;
        avpkt.data += len;
        if (avpkt.size < AUDIO_REFILL_THRESH) {
            /* Refill the input buffer, to avoid trying to decode
             * incomplete frames. Instead of this, one could also use
             * a parser, or use a proper container format through
             * libavformat. */
            memmove(inbuf, avpkt.data, avpkt.size);
            avpkt.data = inbuf;
            len = fread(avpkt.data + avpkt.size, 1,
                        AUDIO_INBUF_SIZE - avpkt.size, f);
            if (len > 0)
                avpkt.size += len;
        }
    }
    
    fclose(outfile);
    fclose(f);
    
    avcodec_close(c);
    av_free(c);
    avcodec_free_frame(&decoded_frame);
}

void AudioLibav::audio_encode_example(const char *filename)
{
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
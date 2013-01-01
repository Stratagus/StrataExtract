#include "AudioLibav.hpp"
AudioLibav::AudioLibav()
{
    
}
AudioLibav::~AudioLibav()
{
    
}
void AudioLibav::ConvertAudio(boost::filesystem::path sourceFilePath, boost::filesystem::path destinationFilePath)
{
    
}
void AudioLibav::audio_decode_example(const char *outfilename, const char *filename)
{
    
    AVCodec *myCodec;
    AVCodecContext *myCodecContext = NULL;
    int outSize, length;
    FILE *fileInput, *fileOutput;
    uint8_t *outputBuffer;
    uint8_t inputBuffer[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    AVPacket avPacket;
    
    
    avcodec_register_all();
    
    av_init_packet(&avPacket);
    std::cout << "Start Audio Decoding\n";
    
    /* Find the PCM WAV 16LE decoder */
    myCodec = avcodec_find_decoder(CODEC_ID_MP2);
    std::cout << "Decoder codec address: " << myCodec << '\n';
    if(!myCodec)
    {
        std::cout << "!!!Could not find the decoder!!!\n";
        exit(-1);
    }
    
    
    myCodecContext = avcodec_alloc_context3(myCodec);
    
    
    if(avcodec_open2(myCodecContext, myCodec, NULL))
    {;
        std::cout << "!!!Codec not found!!\n";
        exit(-1);
    }
    
    //Defered from pattern here
    //std::cout <<
    outputBuffer = reinterpret_cast<uint8_t *>(malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE));
    
    fileInput = fopen(filename, "rb");
    if(!fileInput)
    {
        std::cout << "Failed to open fileInput\n";
        exit(-1);
    }
    fileOutput = fopen(outfilename, "wb");
    if(!fileOutput)
    {
        std::cout << "Failed to open outputfile";
        exit(-1);
    }
    
    
    avPacket.data = inputBuffer;
    avPacket.size = fread(inputBuffer, 1, AUDIO_INBUF_SIZE, fileInput);
    
    while (avPacket.size > 0)
    {
        outSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
        length = avcodec_decode_audio3(myCodecContext, (short *) outputBuffer, &outSize, &avPacket);
        std::cout << "---Decoding\n";
        if(length < 0)
        {
            std::cout << "Error while decoding";
            exit(-1);
        }
        if(outSize > 0)
        {
            //If a frame is decoded, output it
            fwrite(outputBuffer, 1, outSize, fileOutput);
        }
        avPacket.size -= length;
        avPacket.data += length;
        if(avPacket.size < AUDIO_REFILL_THRESH)
        {
            std::cout << "+++Refreshing the buffer\n";
            memmove(inputBuffer, avPacket.data, avPacket.size);
            avPacket.data = inputBuffer;
            length= fread(avPacket.data + avPacket.size, 1, AUDIO_INBUF_SIZE - avPacket.size, fileInput);
            if(length > 0)
            {
                avPacket.size += length;
            }
        }
    }
    fclose(fileOutput);
    fclose(fileInput);
    free(outputBuffer);

}
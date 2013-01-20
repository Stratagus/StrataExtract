#include "main.hpp"

#include "Archives/MPQ/MPQArchive.hpp"
#include "Conversion/Image/ImageMagick/MagickImage.hpp"
#include "Conversion/Audio/FFmpeg/AudioFFmpeg.hpp"
#include "Conversion/Video/FFmpeg/VideoFFmpeg.hpp"


//Test when Stormlib addresses the x64 bit issue.
void testArchiveExtractRaw(MPQArchive *myArchive)
{
    myArchive->ExtractRaw("campaign/terran/terran02/staredit/wav/t2b00tad.wav", "target.wav");
}

void testImage(MPQArchive *myArchive)
{
    MagickImage *myImage = new MagickImage;
    myImage->ReadImageFromMemory(myArchive->ReadFile("glue/create/pinfo.pcx"));
    myImage->WriteImageToFile("blah.png");
    
    delete myImage;
    myImage = NULL;
}

void testDecodeAudio(MPQArchive *myArchive)
{
    AudioFFmpeg *myAudio = new AudioFFmpeg;
    myAudio->DecodeAudio(myArchive->ReadFile("campaign/terran/terran02/staredit/wav/t2b00tad.wav"));
}

void testEncodeAudio(MPQArchive *myArchive)
{
    AudioFFmpeg *myAudio = new AudioFFmpeg;
    myAudio->EncodeAudio();
}


int main(int numberOfArguments, char **commandArguments)
{
    std::cout << "StrataExtract CommandLine\n";
    
    //MPQArchive *myArchive = new MPQArchive;
    //std::cout << "Open Archive stardat.mpq\n";
    
    MPQArchive *myArchive = new MPQArchive;
    myArchive->OpenArchive("starcraft.mpq");
    
        testArchiveExtractRaw(myArchive);
    
        testImage(myArchive);
    
        testDecodeAudio(myArchive);
        //testEncodeAudio(myArchive);
    
    myArchive->CloseArchive();
    delete myArchive;
    myArchive = NULL;
    return 0;
}
#include "main.hpp"

#include "Archives/MPQ/MPQArchive.hpp"
#include "Conversion/Image/ImageMagick/MagickImage.hpp"
#include "Conversion/Audio/Libav/AudioLibav.hpp"
#include "Conversion/Video/Libav/VideoLibav.hpp"


int main(int numberOfArguments, char **commandArguments)
{
    std::cout << "StrataExtract CommandLine\n";
    
    //MPQArchive *myArchive = new MPQArchive;
    //std::cout << "Open Archive stardat.mpq\n";
    
    std::vector<char> vec;
    std::ifstream file;
    file.exceptions(
                    std::ifstream::badbit
                    | std::ifstream::failbit
                    | std::ifstream::eofbit);
    file.open("music.wav");
    file.seekg(0, std::ios::end);
    std::streampos length(file.tellg());
    if (length) {
        file.seekg(0, std::ios::beg);
        vec.resize(static_cast<std::size_t>(length));
        file.read(&vec.front(), static_cast<std::size_t>(length));
    }
    
    
    
    MPQArchive *myArchive = new MPQArchive;
    myArchive->OpenArchive("INSTALL.EXE");
    
    
    
    //MagickImage *myImage = new MagickImage;
    //myImage->ReadImageFromMemory(myArchive->ReadFile("glue/create/pinfo.pcx"));
    //myImage->WriteImageToFile("blah.png");
    
    //delete myImage;
    //myImage = NULL;
    
    AudioLibav *myAudio = new AudioLibav;
    //myAudio->DecodeAudio(myArchive->ReadFile("campaign/terran/terran02/staredit/wav/t2b00tad.wav"));
    myAudio->DecodeAudio(&vec);
    myAudio->EncodeAudio();
    //myAudio->audio_encode_example("junk.mp2");
    
    
    //myAudio->audio_decode_example("testrawout", "test.wav");
    //myAudio->audio_decode_example("musicrawout", "music.wav");
    //myAudio->ConvertAudio("./test.wav", "./test.ogg");
    //myAudio->audio_decode_example("testraw", "test.wav");
    //myAudio->audio_encode_example("tester.mp2");
    
    //VideoLibav *myVideo = new VideoLibav;
    //myVideo->video_decode_example("videorawout", "test.smk");
    
    
    //Test when Stormlib addresses the x64 bit issue.
    //myArchive->ExtractRaw("smk/terran4.smk", "test.smk");
    //myArchive->ExtractRaw("music/zerg3.wav", "music.wav");
    
    
    
    
    
    myArchive->CloseArchive();
    delete myArchive;
    myArchive = NULL;
    return 0;
}
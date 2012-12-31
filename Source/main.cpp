#include "main.hpp"

#include "Archives/MPQ/MPQArchive.hpp"
#include "Conversion/Image/ImageMagick/MagickImage.hpp"


int main(int numberOfArguments, char **commandArguments)
{
    std::cout << "StrataExtract CommandLine\n";
    
    //MPQArchive *myArchive = new MPQArchive;
    //std::cout << "Open Archive stardat.mpq\n";
    
    
    MPQArchive *myArchive = new MPQArchive;
    myArchive->OpenArchive("INSTALL.EXE");
    std::cout << "glue/create/pinfo.pcx FileSize: " << myArchive->GetFileLength("glue/create/pinfo.pcx");
    //std::vector<char> *graphic = ;
    std::vector<char> *video = myArchive->ReadFile("smk/terran4.smk");

    //std::cout << "\nGraphic Vector Size: " << graphic->size() << "\nVideo Vector Size: " << video->size();
 
    delete video;

    MagickImage* myImage = new MagickImage;
    myImage->ReadImageFromMemory(myArchive->ReadFile("glue/create/pinfo.pcx"));
    myImage->WriteImageToFile("blah.png");
    
    delete myImage;
    myImage = NULL;
    
    //myArchive->ExtractRaw("music/zerg3.wav", "./out/music.wav");
    
    
    
    
    
    myArchive->CloseArchive();
    delete myArchive;
    myArchive = NULL;
    return 0;
}
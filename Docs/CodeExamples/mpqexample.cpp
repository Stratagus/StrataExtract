#include "main.hpp"

#include "Archives/MPQ/MPQArchive.hpp"

int main(int numberOfArguments, char **commandArguments)
{
    //Reenable when releasing
    /*if(numberOfArguments < 2)
    {
        StrataConfig::PrintUsage();
        return 0;
    }*/
    
    //Create a StrataConfig object pointer to the singleton to discover what game
    //we are extracting and what "rules" to follow.
    //StrataConfig *Config = StrataConfig::Configuration();
    //StrataRunner *Runner = StrataRunner::Runner();
    
    std::cout << "StrataExtract CommandLine\n";
    
    MPQArchive *myArchive = new MPQArchive;
    MPQArchive *newArchive = new MPQArchive;
    std::cout << "Open Archive stardat.mpq\n";
    
    myArchive->OpenArchive("INSTALL.EXE","list.txt");
    myArchive->OpenArchive("INSTALL.EXE");
    myArchive->ExtractRaw("music/zerg3.wav", "./out/music.wav");
    myArchive->CloseArchive();

    
    
    //newArchive->CreateArchive("newworld.mpq", 100, MPQ_CREATE_ARCHIVE_V1);
    
    
    
    //Config->readConfig("Stargus.StrataExtract");
    //std::cout << "\n" << Config->FindSourcePathHash("/Volumes/STARCRAFT");
    return 0;
}

#include "main.hpp"

#include "Archives/MPQ/MPQArchive.hpp"
#include "Conversion/Image/ImageMagick/MagickImage.hpp"
#include "Conversion/Audio/FFmpeg/AudioFFmpeg.hpp"
#include "Conversion/Video/FFmpeg/VideoFFmpeg.hpp"

#include "Configuration/StrataConfig.hpp"
#include "AssetManager/StrataRunner.hpp"


int main(int numberOfArguments, char **commandArguments)
{
    std::cout << "StrataConfig Tests\n";
    //Create a Strataconfig reference (from teh singleton
    StrataConfig *myConfig = StrataConfig::Configuration();
    
    //The root directory of the soure game
    if(myConfig->setGameMediaSourcePath("/Users/brad/Games/"))
    {
        std::cout << "Set Source path\n";
    }
    else
    {
        std::cout << "SOURCE PATH not set\n";
    }
    
    //Where we want the resulting file to be
    if(myConfig->setGameMediaDestinationPath("/Users/brad/Desktop/"))
    {
        std::cout << "Set Destination path\n";
    }
    else
    {
        std::cout << "DESTINATION PATH not set\n";
    }
    
    //Read the XML configuration
    myConfig->readConfig("../../Docs/SampleConfigs/Stargus.StrataExtract");
    if(myConfig->isExpansionGame())
    {
        std::cout << "You are loading a expansion game, you must give a proper destination path.\n";
    }
    else
    {
        std::cout << "Not a Expansion.\n";
    }
    myConfig->ProcessGameAssetLists();
    //std::cout << "Game Name: " << myConfig->getGameName() << '\n';
    StrataRunner *runner = StrataRunner::Runner();
    std::cout << "Done!\n";
    return 0;
}
#include "main.hpp"

#include "Archives/MPQ/MPQArchive.hpp"
#include "Conversion/Image/ImageMagick/MagickImage.hpp"
#include "Conversion/Audio/FFmpeg/AudioFFmpeg.hpp"
#include "Conversion/Video/FFmpeg/VideoFFmpeg.hpp"

#include "Configuration/StrataConfig.hpp"
#include "AssetManager/StrataRunner.hpp"


int main(int numberOfArguments, char **commandArguments)
{
    //Create a Strataconfig reference (from teh singleton
    StrataConfig *mainConfig = StrataConfig::Configuration();
    mainConfig->PrintUsage();
    
    return 0;
}
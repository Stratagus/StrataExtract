#include "StrataConfig.hpp"

StrataConfig::StrataConfig()
{
    totalObjects = 0;
    completeObjects = 0;
    configLoaded = false;
}

StrataConfig* StrataConfig::ConfigurationInstance = 0;

StrataConfig *StrataConfig::Configuration()
{
    if(ConfigurationInstance == 0)
    {
        ConfigurationInstance = new StrataConfig;
    }
    
    return ConfigurationInstance;
}

void StrataConfig::readConfig(boost::filesystem::path configurationPath)
{
    if(!boost::filesystem::exists(configurationPath))
    {
        std::cerr << "File not found " << configurationPath;
    }
    else
    {
        //The XML document file
        xmlDoc *configurationDocument = NULL;
        //The root of our configuration tree
        
        
        configurationDocument = xmlReadFile(configurationPath.string().c_str(), NULL, 0);
        
        //Ensure that the file we read is a valid StrataExtractConfig and not simply a XML file.
        if(xmlStrcmp(xmlDocGetRootElement(configurationDocument)->name, (const xmlChar *) "StrataExtractConfig"))
        {
            std::cerr << "Document is not a StrataExtraction Config\n";
        }
        else
        {
            rootConfigElement = xmlDocGetRootElement(configurationDocument)->children;
            
            for(xmlNode *interationElement = rootConfigElement; interationElement;interationElement = interationElement->next)
            {
                std::cout << "\nCurrent Interation Value: " << interationElement->children << '\n';
                /*if(xmlStrcmp(interationElement->name, (const xmlChar *) "MapAssets"))
                    MapAssetsElement = interationElement;
                    else if (xmlStrcmp(interationElement->name, (const xmlChar *) "AudioAssets"))
                            AudioAssetsElement = interationElement;
                        else if(xmlStrcmp(interationElement->name, (const xmlChar *) "ImageAssets"))
                                ImageAssetsElement = interationElement;
                            else if(xmlStrcmp(interationElement->name, (const xmlChar *) "VideoAssets"))
                                VideoAssetsElement = interationElement;
                                else if(xmlStrcmp(interationElement->name, (const xmlChar *) "FontAssets"))
                                    ExtractAssetsElement = interationElement;
                                    else if(xmlStrcmp(interationElement->name, (const xmlChar *) "ExtractAssets"))
                                        ExtractAssetsElement = interationElement;
                                    else{}*/
            }
            
            //std::cout << "\nElements found with " << MapAssetsElement->name << '\n' << AudioAssetsElement << '\n' << ImageAssetsElement << '\n' << VideoAssetsElement << '\n';
            //The Configuration was successfully parsed and loaded
            configLoaded = true;
        }
        
        //free the document
        xmlFreeDoc(configurationDocument);
        
        //Free the global variables that may
        //have been allocated by the parser.
        xmlCleanupParser();
    }
}

int StrataConfig::GetCPUCores()
{
   return boost::thread::hardware_concurrency();
}

float StrataConfig::GetProgess()
{    
    if(totalObjects == 0)
        return -1; //The configration has not been loaded.
    else
        return floor( ((float)completeObjects/ (float)totalObjects) * 10000 ) / 100;
}

bool StrataConfig::isConfigLoaded()
{
    return configLoaded;
}

void StrataConfig::GetDestinationAudioCodec()
{
    if(!isConfigLoaded())
    {
        std::cerr << "Configuration is not loaded!";
    }
    else
    {
        //xmlNode *currentNode = rootConfigElement;
        
        //for(; xmlStrcmp(currentNode->name, (const xmlChar *) "AudioAssets"); currentNode = currentNode->next)
        //{
        //    std::cout << "\nRunning through the list";
        //}
        
        std::cout << "Destination Audio Codec:  ";
    }
}
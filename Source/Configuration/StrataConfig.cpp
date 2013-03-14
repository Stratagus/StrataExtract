#include "StrataConfig.hpp"

StrataConfig::StrataConfig()
{
    totalObjects = 0;
    completeObjects = 0;
    configLoaded = false;
    isExpansion = false;
    configurationDocument = NULL;
    configXPathContext = NULL;
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

        
        //The root of our configuration tree
        configurationDocument = xmlReadFile(configurationPath.string().c_str(), NULL, NULL);
        if(configurationDocument == NULL)
        {
            std::cerr << "Error: Unable to parse file: " << configurationPath << '\n';
            exit(-1);
        }
        
        //Ensure that the file we read is a valid StrataExtractConfig and not simply a XML file.
        if(xmlStrcmp(xmlDocGetRootElement(configurationDocument)->name, (const xmlChar *) "StrataExtractConfig"))
        {
            std::cerr << "Document is not a StrataExtraction Config\n";
        }
        else
        {
            configXPathContext = xmlXPathNewContext(configurationDocument);
            configurationRoot = xmlDocGetRootElement(configurationDocument);
        }
        
        gameVersion = FindGameVersion();
        
        if(!gameVersion)
        {
            throw "No valid version detected";
        }
        
        
        //free the document
        //xmlFreeDoc(configurationDocument);
        //configurationDocument = NULL;
        
        //Free the global variables that may
        //have been allocated by the parser.
        //xmlCleanupParser();
        
        configLoaded = 1;
    }
}

xmlNodePtr StrataConfig::FindGameVersion()
{
    //Jump to the <Versions> entity
    xmlXPathObjectPtr gameVersions = xmlXPathEval((const xmlChar *) "//Version", configXPathContext);
    
    bool fileMatch = true;
    
    //Interate through each version (<Version> entities) of the game until we hit a match or fail 
    for(int numberOfVersions = 0; numberOfVersions < (gameVersions->nodesetval->nodeNr); numberOfVersions++)
    {
        xmlNodePtr currentNodePointer = gameVersions->nodesetval->nodeTab[numberOfVersions];
        xmlNodePtr currentChildPointer = currentNodePointer->children->next;
        
        //Reset the fileMatch flag
        fileMatch = true;
        
        //Start checking the <file> hashes with those in this version, if ANY fail break out
        while(currentChildPointer != NULL && fileMatch)
        {
            
        #warning Potential logic error (Code Review)
            //Compare the Hash in the current file entity with the method generated hash
            if(!xmlStrcmp(xmlGetProp(currentChildPointer, (const xmlChar *) "hash"), GetFileHash((char *)xmlGetProp(currentChildPointer, (const xmlChar *) "name"))))
            {
                if(currentChildPointer->next->next == NULL)
                {
                   //Check if the GameMediaSource is a expansion (We will need additional input)
                   if(!xmlStrcmp(xmlGetProp(currentNodePointer, (const xmlChar *) "expansion"), (xmlChar *) "1"))
                   {
                       isExpansion = true;
                   }
                   //If everything checks out and all files are accounted for with the proper SHA1 hashes return
                   //the game version for the configuration
                   return gameVersions->nodesetval->nodeTab[numberOfVersions];
                }
            }
            else
            {
                fileMatch = false;
            }
        #warning This line probably breaks something, but I am not sure (Code Review)
            currentChildPointer = currentChildPointer->next->next;
        }
    }
    return NULL;
}

void StrataConfig::ProcessGameAssetLists()
{
    xmlNodePtr gameVersionFilePointer = gameVersion->children->next;
    std::cout << "Game Version is: " << xmlGetProp(gameVersion, (const xmlChar *) "name") << '\n';
    while(gameVersionFilePointer != NULL)
    {
        if(xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Archive") != NULL)
        {
            std::cout << "It's a archive file!!\n";
            ProcessArchive(LookupArchive(xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Archive")));
        }
        gameVersionFilePointer = gameVersionFilePointer->next->next;
    }
    
}

void StrataConfig::ProcessArchive(xmlNodePtr archive)
{
    //Set the "scope" in which we want to the XPath expression to evaluate
    configXPathContext->node = archive;
    xmlXPathObjectPtr archiveAssets = xmlXPathEval((const xmlChar *) "*", configXPathContext);

    #warning Need a more efficent way to do this (Code Review)
    xmlChar* assetTypes [8] = { (xmlChar *)"ArchiveAssets", (xmlChar *)"AudioAssets",
                               (xmlChar *)"MapAssets", (xmlChar *)"ImageAssets",
                               (xmlChar *)"VideoAssets", (xmlChar *)"FontAssets",
                               (xmlChar *)"CampaignAssets", (xmlChar *)"ExtractAssets"};
    
    for(int currentAssetType = 0; currentAssetType < 8; currentAssetType++)
    {
        archiveAssets = xmlXPathEval(assetTypes[currentAssetType], configXPathContext);
        if(archiveAssets->nodesetval->nodeTab > 0)
        {
                 std::cout << "Got a for typeset " << assetTypes[currentAssetType] << '\n';
                if(currentAssetType > 0)
                {
                    processQueue.push(archiveAssets->nodesetval->nodeTab[0]->children->next);
                }
                else
                {
                    xmlNodePtr archiveAssetLists = archiveAssets->nodesetval->nodeTab[0]->children->next;
                    while (archiveAssetLists != NULL)
                    {
                        preparationProcessQueue.push(LookupArchive(xmlGetProp(archiveAssetLists, (const xmlChar *) "Archive")));
                        ProcessArchive(LookupArchive(xmlGetProp(archiveAssetLists, (const xmlChar *) "Archive")));
                        archiveAssetLists = archiveAssetLists->next->next;
                    }
                }
            
        }
        configXPathContext->node = archive;
    }
}

xmlNodePtr StrataConfig::LookupArchive(xmlChar* archiveName)
{
    configXPathContext->node = configurationRoot;
    xmlXPathObjectPtr archiveList = xmlXPathEval((const xmlChar *) "//Archive", configXPathContext);
    for(int junk = 0; junk < archiveList->nodesetval->nodeNr; junk++)
    {
        //std::cout << archiveList->nodesetval->nodeTab[junk]->name << '\n';
        if(!xmlStrcmp(xmlGetProp(archiveList->nodesetval->nodeTab[junk], (const xmlChar *) "name"), archiveName))
        {
            //printf("%s\n", xmlGetProp(archiveList->nodesetval->nodeTab[junk], (const xmlChar *) "name"));
            return archiveList->nodesetval->nodeTab[junk];
        }
        
    }
    return NULL;
}

bool StrataConfig::isExpansionGame()
{
    if(!isConfigLoaded())
    {
        throw "No configuration loaded in strataconfig";
    }
    
    return isExpansion;
}

bool StrataConfig::setGameMediaSourcePath(boost::filesystem::path gameMediaSourcePath)
{
    if(!boost::filesystem::exists(gameMediaSourcePath) || !boost::filesystem::is_directory(gameMediaSourcePath))
    {
        return false;
    }
    
    gameMediaSource = gameMediaSourcePath;
    return true;
}
bool StrataConfig::setGameMediaDestinationPath(boost::filesystem::path gameMediaDestinationPath)
{
    if(boost::filesystem::exists(gameMediaDestinationPath))
    {
        return false;
    }
    
    gameMediaDestination = gameMediaDestinationPath;
    return true;
}

boost::filesystem::path StrataConfig::GameMediaSourcePath()
{
    return gameMediaSource;
}
boost::filesystem::path StrataConfig::GameMediaDestinationPath()
{
    return gameMediaDestination;
}

/*std::string StrataConfig::getGameName()
{
    if(!isConfigLoaded())
    {
        throw "GetGameName: No config loaded";
    }
    configXPathContext->node = configurationRoot;
    xmlXPathObjectPtr result = xmlXPathEval((xmlChar *) "title", configXPathContext);
    if(result == NULL)
    {
        throw "Incomplete game configuration";
    }
    std::cout << "Test: " << result->nodesetval->nodeTab[0]->name << '\n';
    return "";
}*/

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

xmlChar* StrataConfig::GetFileHash(boost::filesystem::path filePath)
{
    std::cout << "Passed in: " << filePath << '\n';
    //SHA1 hash here
    //return NULL;
    return (xmlChar*) "2de01f59e99c0fb16d32df2d7cdd909be2bf0825";
}

/*std::string StrataConfig::FindSourcePathHash(boost::filesystem::path gamePath)
{
    if(configurationRoot == NULL)
    {
        std::cerr << "Configuration file not read.";
    }
    return "";
}*/

/*std::string StrataConfig::FindSourcePathHash(boost::filesystem::path gamePath)
{
    boost::uuids::detail::sha1 myHash;
    boost::filesystem::ifstream hashFile;
    std::stringstream finalHash;
    std::string hello = "Hello World";
    char hash[20];
    
    
    myHash.process_bytes(hello.c_str(), hello.size());
    
    
    unsigned int digest[5];
    myHash.get_digest(digest);
    for(int i = 0; i < 5; ++i)
    {
        const char* tmp = reinterpret_cast<char*>(digest);
        hash[i*4] = tmp[i*4+3];
        hash[i*4+1] = tmp[i*4+2];
        hash[i*4+2] = tmp[i*4+1];
        hash[i*4+3] = tmp[i*4];
    }
    
    finalHash << std::hex;
    for(int i = 0; i < 20; ++i)
    {
        finalHash << ((hash[i] & 0x000000F0) >> 4)
        << (hash[i] & 0x0000000F);
    }
    std::cout << finalHash.str();
    return "";
}*/

/**
 * print_xpath_nodes:
 * @nodes:		the nodes set.
 * @output:		the output file handle.
 *
 * Prints the @nodes content to @output.
 */
void StrataConfig::print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output) {
    xmlNodePtr cur;
    int size;
    int i;
    
    assert(output);
    size = (nodes) ? nodes->nodeNr : 0;
    
    fprintf(output, "Result (%d nodes):\n", size);
    for(i = 0; i < size; ++i) {
        assert(nodes->nodeTab[i]);
        
        if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
            xmlNsPtr ns;
            
            ns = (xmlNsPtr)nodes->nodeTab[i];
            cur = (xmlNodePtr)ns->next;
            if(cur->ns) {
                fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n",
                        ns->prefix, ns->href, cur->ns->href, cur->name);
            } else {
                fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n",
                        ns->prefix, ns->href, cur->name);
            }
        } else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
            cur = nodes->nodeTab[i];
            if(cur->ns) {
    	        fprintf(output, "= element node \"%s:%s\"\n",
                        cur->ns->href, cur->name);
            } else {
    	        fprintf(output, "= element node \"%s\"\n",
                        cur->name);
            }
        } else {
            cur = nodes->nodeTab[i];    
            fprintf(output, "= node \"%s\": type %d\n", cur->name, cur->type);
        }
    }
}

void StrataConfig::print_xpath(xmlNodeSetPtr nodes) {
    xmlNodePtr cur;
    int size;
    int i;
    
    size = (nodes) ? nodes->nodeNr : 0;
    
    printf("Result (%d nodes):\n", size);
    for(i = 0; i < size; ++i) {
        assert(nodes->nodeTab[i]);
        
        if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
            xmlNsPtr ns;
            
            ns = (xmlNsPtr)nodes->nodeTab[i];
            cur = (xmlNodePtr)ns->next;
            if(cur->ns) {
                printf("= namespace \"%s\"=\"%s\" for node %s:%s\n",
                        ns->prefix, ns->href, cur->ns->href, cur->name);
            } else {
                printf( "= namespace \"%s\"=\"%s\" for node %s\n",
                        ns->prefix, ns->href, cur->name);
            }
        } else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
            cur = nodes->nodeTab[i];
            if(cur->ns) {
    	        printf( "= element node \"%s:%s\"\n",
                        cur->ns->href, cur->name);
            } else {
    	        printf( "= element node \"%s\"\n",
                        cur->name);
            }
        } else {
            cur = nodes->nodeTab[i];
            printf("= node \"%s\": type %d\n", cur->name, cur->type);
        }
    }
}
void StrataConfig::PrintUsage()
{
    std::cout << "Stratagus Version " << VERSION << ' '
              << "Copyright (c) 2012 by StrataExtract Team\n\n"
              << "Usage:./strataextract [OPTIONS] /gameconfig/path /source/game/path /destination/game/path\n"
              << "Flags             Describution\n"
              << "--gui             Open GUI\n"
              << "--noconvert       Ignore Conversion tags\n"
              << "--simulate        Simulate game extraction\n"
              << "--verbosity #     0-4 Display internal operations\n"
              << "                  0 - Nothing\n"
              << "                  1 - Errors\n"
              << "                  2 - Warnings (Default)\n"
              << "                  3 - Main Operations\n"
              << "                  4 - All Operations\n";
}

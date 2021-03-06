#include "StrataConfig.hpp"

StrataConfig::StrataConfig()
{
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Constructing StratConfig Object";
    totalObjects = 0;
    completeObjects = 0;
    configLoaded = false;
    isExpansion = false;
    configurationDocument = nullptr;
    configXPathContext = nullptr;
    
    gameVersion = nullptr;
    configurationRoot = nullptr;
    gameConfiguration = nullptr;
    gameMediaSource = nullptr;
    gameMediaDestination = nullptr;
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Finished StratConfig Object";
}

StrataConfig::~StrataConfig()
{
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Deconstructing StratConfig Object";
    if(configurationDocument)
    {
        delete configurationDocument;
        configurationDocument = nullptr;
    }
    
    if (configXPathContext)
    {
        delete configXPathContext;
        configXPathContext = nullptr;
    }
    
    if(gameConfiguration)
    {
        delete gameConfiguration;
        gameConfiguration = nullptr;
    }
    
    if(gameMediaSource)
    {
        delete gameMediaSource;
        gameMediaSource = nullptr;
    }
    
    if(gameMediaDestination)
    {
        delete gameMediaDestination;
        gameMediaDestination = nullptr;
    }
    
    if(configurationDocument)
    {
        xmlFreeDoc(configurationDocument);
        configurationDocument = nullptr;
    }
    
    //Free the global variables that may
    //have been allocated by the parser.
    xmlCleanupParser();
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Deconstructing StratConfig Object";
}

bool StrataConfig::readConfig()
{
    if(!gameConfiguration)
    {
        StrataConfigFilesystemException fileNotFound;
        //fileNotFound.SetErrorMessage("Game configuration file path not set");
        gameConfiguration = new boost::filesystem::path;
        fileNotFound.problemPath = gameConfiguration;
        BOOST_LOG_SEV(configLogger, boost::log::trivial::debug) << "Game configuration file was not set.";
        BOOST_THROW_EXCEPTION(fileNotFound);
        
    }
    if(!boost::filesystem::exists(*gameConfiguration))
    {
        BOOST_LOG_SEV(configLogger, boost::log::trivial::error) << "Game configuration file was not found.";
    }
    else
    {

        
        //The root of our configuration tree
        configurationDocument = xmlReadFile(gameConfiguration->string().c_str(), NULL, NULL);
        if(configurationDocument == nullptr)
        {
            StrataConfigParsingException parseReadError;
            BOOST_LOG_SEV(configLogger, boost::log::trivial::error) << "Unable to parse the configuration";
            BOOST_THROW_EXCEPTION(parseReadError);
            return false;
        }
        
        //Ensure that the file we read is a valid StrataExtractConfig and not simply a XML file.
        BOOST_LOG_SEV(configLogger, boost::log::trivial::debug) << "Looking for XML element \"StrataExtractConfig\".";
        if(xmlStrcmp(xmlDocGetRootElement(configurationDocument)->name, (const xmlChar *) "StrataExtractConfig"))
        {
            StrataConfigParsingException notStrataConfig;
            BOOST_LOG_SEV(configLogger, boost::log::trivial::error) << "Document is not a StrataExtraction Config.";
            BOOST_THROW_EXCEPTION(notStrataConfig);
        }
        else
        {
            BOOST_LOG_SEV(configLogger, boost::log::trivial::debug) << "Setting up XPaths to search through the configuration.";
            configXPathContext = xmlXPathNewContext(configurationDocument);
            configurationRoot = xmlDocGetRootElement(configurationDocument);
        }
        
        gameVersion = FindGameVersion();
        
        if(!gameVersion)
        {
            StrataConfigParsingException detectionException;
            detectionException.SetErrorMessage("No valid game version detected.");
            BOOST_THROW_EXCEPTION(detectionException);
        }
        
        BOOST_LOG_SEV(configLogger, boost::log::trivial::info) << "Successfully loaded StrataExtract configuration.";
        configLoaded = 1;
        return true;
    }
    return false;
}

xmlNodePtr StrataConfig::FindGameVersion()
{
#warning Check that the source path is loaded
    //Jump to the <Versions> entity
    boost::filesystem::path tempSourceGamePath = *gameMediaSource;
    xmlXPathObjectPtr gameVersions = xmlXPathEval((const xmlChar *) "//Version", configXPathContext);
    
    if(gameVersions == nullptr)
    {
        BOOST_LOG_SEV(configLogger, boost::log::trivial::error) << "\"Version\" was not found";
        StrataConfigParsingException noVersionTag;
        noVersionTag.SetErrorMessage("No version tag detected");
        BOOST_THROW_EXCEPTION(noVersionTag);
    }
    
    bool fileMatch = true;
    
    //Interate through each version (<Version> entities) of the game until we hit a match or fail 
    for(int numberOfVersions = 0; numberOfVersions < (gameVersions->nodesetval->nodeNr); numberOfVersions++)
    {
        xmlNodePtr currentNodePointer = gameVersions->nodesetval->nodeTab[numberOfVersions];
        xmlNodePtr currentChildPointer = currentNodePointer->children->next;
        
        //Reset the fileMatch flag
        fileMatch = true;
        
        //Start checking the <file> hashes with those in this version, if ANY fail break out
        while(currentChildPointer != nullptr && fileMatch)
        {
        #warning Potential logic error (Code Review)
            //Compare the Hash in the current file entity with the method generated hash
            BOOST_LOG_SEV(configLogger, boost::log::trivial::debug) << "Comparing Hash from Config("
                                                                    << (char *) xmlGetProp(currentChildPointer, (const xmlChar *) "name") << ')' 
                                                                    << (char *) xmlGetProp(currentChildPointer, (const xmlChar *) "hash") << '\n'
                                                                   << " with Generated Hash: " << (char *) GetFileHash(*gameMediaSource / (char *)xmlGetProp(currentChildPointer, (const xmlChar *) "name")) ;
            if(!xmlStrcmp(xmlGetProp(currentChildPointer, (const xmlChar *) "hash"), GetFileHash(*gameMediaSource / (char *)xmlGetProp(currentChildPointer, (const xmlChar *) "name"))))
            {
                if(currentChildPointer->next->next == nullptr)
                {
                   //Check if the GameMediaSource is a expansion (We will need additional input)
                   if(!xmlStrcmp(xmlGetProp(currentNodePointer, (const xmlChar *) "expansion"), (xmlChar *) "1"))
                   {
                       BOOST_LOG_SEV(configLogger, boost::log::trivial::info) << "Game is a Expansion";
                       isExpansion = true;
                   }
                   //If everything checks out and all files are accounted for with the proper SHA1 hashes return
                   //the game version for the configuration
                    BOOST_LOG_SEV(configLogger, boost::log::trivial::info) << "Game Version is " << (char *)xmlGetProp(gameVersions->nodesetval->nodeTab[numberOfVersions], (const xmlChar *) "name");
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
    BOOST_LOG_SEV(configLogger, boost::log::trivial::error) << "No game version detected";
    return nullptr;
}

void StrataConfig::ProcessGameAssetLists()
{
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "ProcessGameAssetLists with GameVersion: " << (char *) xmlGetProp(gameVersion, (const xmlChar *) "name");
    xmlNodePtr gameVersionFilePointer = gameVersion->children->next;
    while(gameVersionFilePointer != nullptr)
    {
        if(xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Archive"))
        {
            BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Processing Archivefile: " << (char *) xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Archive");
            ProcessArchive(LookupArchive(xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Archive")));
        }
        
        if(xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Copy"))
        {
            totalObjects++;
            BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Copying file: " << (*gameMediaSource / ((char *) xmlGetProp(gameVersionFilePointer, (const xmlChar *) "name")))
            << " to: " << (*gameMediaDestination / ((char *) xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Copy")));
            if(!gameMediaDestination)
            {
                StrataConfigFilesystemException destinationFilePathNotSet;
                destinationFilePathNotSet.SetErrorMessage("Destination file path not set");
                destinationFilePathNotSet.problemPath = gameMediaDestination;
                BOOST_THROW_EXCEPTION(destinationFilePathNotSet);
            }
            else
            {
                if(!boost::filesystem::exists(*gameMediaDestination))
                {
                    if(!boost::filesystem::create_directory(*gameMediaDestination))
                    {
                        StrataConfigFilesystemException couldNotCreateDirectory;
                        couldNotCreateDirectory.SetErrorMessage("Could not create directory");
                        couldNotCreateDirectory.problemPath = gameMediaDestination;
                        BOOST_THROW_EXCEPTION(couldNotCreateDirectory);
                    }
                }
                boost::filesystem::copy((*gameMediaSource / ((char *) xmlGetProp(gameVersionFilePointer, (const xmlChar *) "name"))), (*gameMediaDestination / ((char *) xmlGetProp(gameVersionFilePointer, (const xmlChar *) "Copy"))));
                totalObjects++;
                completeObjects++;
            }
        }
        gameVersionFilePointer = gameVersionFilePointer->next->next;
    }
    
}

void StrataConfig::ProcessArchive(xmlNodePtr archive)
{
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Entering ProcessArchive with: " << (char *) xmlGetProp(archive, (const xmlChar *) "name");
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
                 //std::cout << "Got a for typeset " << assetTypes[currentAssetType] << '\n';
            BOOST_LOG_SEV(configLogger, boost::log::trivial::debug) << "Found Tag " << archiveAssets->nodesetval->nodeNr << " of type " << (char *) assetTypes[currentAssetType] << " with "
                                                                    << xmlChildElementCount(archiveAssets->nodesetval->nodeTab[0]) << " objects.";
                if(currentAssetType > 0)
                {
                    for(int currentAssetTagProcess = 0; currentAssetTagProcess < archiveAssets->nodesetval->nodeNr; currentAssetTagProcess++)
                    {
                        totalObjects += xmlChildElementCount(archiveAssets->nodesetval->nodeTab[0]);
                       processQueue.push(archiveAssets->nodesetval->nodeTab[currentAssetTagProcess]->children->next);
                    }
                }
                else
                {
                    xmlNodePtr archiveAssetLists = archiveAssets->nodesetval->nodeTab[0]->children->next;
                    while (archiveAssetLists != nullptr)
                    {
                        preparationProcessQueue.push(LookupArchive(xmlGetProp(archiveAssetLists, (const xmlChar *) "Archive")));
                        ProcessArchive(LookupArchive(xmlGetProp(archiveAssetLists, (const xmlChar *) "Archive")));
                        archiveAssetLists = archiveAssetLists->next->next;
                        BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Returning ProcessArchive with: " << (char *) xmlGetProp(archive, (const xmlChar *) "name");
                    }
                }
            
        }
        configXPathContext->node = archive;
    }
}

xmlNodePtr StrataConfig::LookupArchive(xmlChar* archiveName)
{
    configXPathContext->node = configurationRoot;
    std::string xPathQuery = "//Archive[@name='";
    xPathQuery.append((char *) archiveName);
    xPathQuery.append("\']");

    xmlXPathObjectPtr archiveList = xmlXPathEval((const xmlChar *) xPathQuery.c_str(), configXPathContext);
    BOOST_LOG_SEV(configLogger, boost::log::trivial::trace) << "Searched with query: " << xPathQuery << "with " << archiveList->nodesetval->nodeNr << " results";
    
    if(archiveList->nodesetval->nodeNr == 1)
    {
        return archiveList->nodesetval->nodeTab[0];
    }
#warning Throw error
    return nullptr;

}

bool StrataConfig::isExpansionGame()
{
    if(!isConfigLoaded())
    {
        StrataConfigNotLoadedException errorInstance;
        errorInstance.SetErrorMessage("No configuration loaded to check for expansion");
        BOOST_THROW_EXCEPTION(errorInstance);
        return false;
    }
    
    return isExpansion;
}

bool StrataConfig::setGameMediaSourcePath(boost::filesystem::path gameMediaSourcePath)
{    
    if(!gameMediaSource)
    {
        gameMediaSource = new boost::filesystem::path;
    }
    
    if(!boost::filesystem::exists(gameMediaSourcePath) || !boost::filesystem::is_directory(gameMediaSourcePath))
    {
        //Wrapping the data we want to hand back to catch
        StrataConfigFilesystemException  fileException;
        fileException.problemPath = gameMediaSource;
        fileException.SetErrorMessage("File path does not exist or is not a directory.");
        
        BOOST_THROW_EXCEPTION(fileException);
        return false;
    }
    
    *gameMediaSource = gameMediaSourcePath;
    return true;
}

bool StrataConfig::setGameConfiguration(boost::filesystem::path gameConfigurationPath)
{
    if(!gameConfiguration)
    {
        gameConfiguration = new boost::filesystem::path;
    }
    
    if(!boost::filesystem::exists(gameConfigurationPath))
    {
    	 BOOST_LOG_SEV(configLogger, boost::log::trivial::error) << "Configuration file path does not exist or is a directory.";
        StrataConfigFilesystemException fileException;
        fileException.problemPath = gameConfiguration;
        fileException.SetErrorMessage("Configuration file path does not exist or is a directory.");
        
        BOOST_THROW_EXCEPTION(fileException);
        return false;
    }
    
    *gameConfiguration = gameConfigurationPath;
    return true;
}

bool StrataConfig::setGameMediaDestinationPath(boost::filesystem::path gameMediaDestinationPath)
{
    if(!gameMediaDestination)
    {
        gameMediaDestination = new boost::filesystem::path;
    }
    
    if(boost::filesystem::exists(gameMediaDestinationPath))
    {
        StrataConfigFilesystemException fileException;
        fileException.problemPath = gameMediaDestination;
        fileException.SetErrorMessage("File path exists or is not a directory.");
        
        BOOST_THROW_EXCEPTION(fileException);
        return false;
    }
    
    *gameMediaDestination = gameMediaDestinationPath;
    return true;
}

boost::filesystem::path StrataConfig::GameMediaSourcePath()
{
    if(gameMediaSource)
    {
        return *gameMediaSource;
    }
    return "";
}
boost::filesystem::path StrataConfig::GameMediaDestinationPath()
{
    if(gameMediaDestination)
    {
        return *gameMediaDestination;
    }
    return "";
}

boost::filesystem::path StrataConfig::GameConfigurationPath()
{
    if(gameConfiguration)
    {
        return *gameConfiguration;
    }
    return "";
}

std::string StrataConfig::getGameName()
{
    if(!isConfigLoaded())
    {
        StrataConfigException noConfigLoaded;
        noConfigLoaded.SetErrorMessage("No configuration no loaded.");
        BOOST_THROW_EXCEPTION(noConfigLoaded);
    }
    configXPathContext->node = configurationRoot;
    xmlXPathObjectPtr result = xmlXPathEval((xmlChar *) "title", configXPathContext);
    if(result == nullptr)
    {
        StrataConfigParsingException parsingError;
        parsingError.SetErrorMessage("Incomplete Game Configuration");
        BOOST_THROW_EXCEPTION(parsingError);
    }
    return (std::string) ((char *)xmlGetProp(gameVersion, (const xmlChar *) "name"));
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

void StrataConfig::ClearCurrentConfigurationDocument()
{
    if(configurationDocument)
    {
        xmlFreeDoc(configurationDocument);
        configurationDocument = nullptr;
    }
    xmlCleanupParser();
}

//If OpenSSL is installed prefer it's hashing library, as it is much
//faster. Else we revert to abusing boost's uuid sha1 hashing
#if OPENSSL_INSTALLED
xmlChar* StrataConfig::GetFileHash(boost::filesystem::path filePath)
{
    //std::cout << "Passing: " << filePath << '\n';
    FILE *f;
    unsigned char buf[8192];
    SHA_CTX sc;
    int err;
    unsigned char output[20];
    std::stringstream finalHash;
    
    f = fopen(filePath.string().c_str(), "rb");
    if (f == nullptr)
    {
        //StrataConfigFilesystemException fileNotFoundError;
        //fileNotFoundError.SetErrorMessage("Unable to open file.");
        //fileNotFoundError.problemPath = &filePath;
        //BOOST_THROW_EXCEPTION(fileNotFoundError);
        return (xmlChar *) "";
    }
    SHA1_Init(&sc);
    for (;;) {
        size_t len;
        
        len = fread(buf, 1, sizeof buf, f);
        if (len == 0)
            break;
        SHA1_Update(&sc, buf, len);
    }
    err = ferror(f);
    fclose(f);
    if (err) {
        //std::cout << "Fail!\n";
        StrataConfigFilesystemException fileIOError;
        fileIOError.SetErrorMessage("Unknown IO Error");
        fileIOError.problemPath = &filePath;
        BOOST_THROW_EXCEPTION(fileIOError);
        
        return (xmlChar *) "";
    }
    SHA1_Final(output, &sc);
    
    for(int i = 0; i < 20; ++i)
    {
        finalHash << std::hex << ((output[i] & 0x000000F0) >> 4)
        << (output[i] & 0x0000000F);
    }
    return (xmlChar *) finalHash.str().c_str();
}
#else
#warning Very Wasteful function in memory and time in the way sha1Digest.process_bytes operates!
xmlChar* StrataConfig::GetFileHash(boost::filesystem::path filePath)
{
    BOOST_LOG_SEV(configLogger, boost::log::trivial::warning) << "Using Boost's SHA1 hashing, will dramatically increase calculation time.";
    //std::cout << "Passing: " << filePath << '\n';
    boost::uuids::detail::sha1 sha1Digest;
    std::stringstream finalHash;
    char hash[20];
    char fileBuffer[2048];
    unsigned int digest[5];
    
    std::ifstream targetFile(filePath.string().c_str(),std::ios::binary);

    //If the file could not be opened return nullptr
    if(!targetFile.good())
    {
        //StrataConfigFilesystemException fileNotFoundError;
        //fileNotFoundError.SetErrorMessage("Unable to open file.");
        //fileNotFoundError.problemPath = &filePath;
        //BOOST_THROW_EXCEPTION(fileNotFoundError);
        return nullptr;
    }
    
    while(targetFile.good())
    {
        targetFile.read(fileBuffer, sizeof(fileBuffer));
        sha1Digest.process_bytes(fileBuffer, targetFile.gcount());
    }
    
    //There was a failure to read the file
    if(!targetFile.eof())
    {
        StrataConfigFilesystemException fileIOError;
        fileIOError.SetErrorMessage("Unknown IO Error");
        fileIOError.problemPath = &filePath;
        BOOST_THROW_EXCEPTION(fileIOError);
        return nullptr;
    }

    //Now that we have the file processed into the digest, close the file
    targetFile.close();
    
    sha1Digest.get_digest(digest);

    for(int i = 0; i < 5; ++i)
    {
        const char* tmp = reinterpret_cast<char *>(digest);
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
    
    return (xmlChar *) finalHash.str().c_str();
}
#endif

void StrataConfig::PrintUsage()
{
    std::cout << "Stratagus Version " << VERSION << ' '
              << "Copyright (c) 2012 by StrataExtract Team\n\n"
              << "Usage:./strataextract [OPTIONS] /gameconfig/path /source/game/path /destination/game/path\n"
              << "Flags             Describution\n"
              << "--noconvert       Ignore Conversion tags\n"
              << "--simulate        Simulate game extraction\n"
              << "--verbosity #     0-4 Display internal operations\n"
              << "                  0 - Nothing\n"
              << "                  1 - Errors\n"
              << "                  2 - Warnings (Default)\n"
              << "                  3 - Main Operations\n"
              << "                  4 - All Operations\n";
}

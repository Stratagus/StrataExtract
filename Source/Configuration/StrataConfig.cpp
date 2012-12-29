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
        configurationDocument = xmlReadFile(configurationPath.string().c_str(), NULL, 33);
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
            xmlXPathContextPtr xPathContext = xmlXPathNewContext(configurationDocument);
            if(xPathContext == NULL)
            {
                std::cout << "Error: Unable to create xpath context." << '\n';
                exit(-1);
            }
            
            audioAssetObjects = xmlXPathEval((const xmlChar *) "//audio", xPathContext);
            mapAssetObjects = xmlXPathEval((const xmlChar *) "//map", xPathContext);
            fontAssetObjects = xmlXPathEval((const xmlChar *) "//font", xPathContext);
            videoAssetObjects = xmlXPathEval((const xmlChar *) "//video", xPathContext);
            tilesetAssetObjects = xmlXPathEval((const xmlChar *) "//tileset", xPathContext);
            
            
            //xmlNodePtr *myAudio = audioAssetObjects->nodesetval->nodeTab;
            //std::cout <<  myAudio[1]->name;
            
            std::cout << "\n# of AudioAssets: " << audioAssetObjects->nodesetval->nodeNr << '\n'
                      << "# of MapAssets: " << mapAssetObjects->nodesetval->nodeNr << '\n'
                      << "# of FontAssets: " << fontAssetObjects->nodesetval->nodeNr << '\n'
                      << "# of VideoAssets: " << videoAssetObjects->nodesetval->nodeNr << '\n'
                      << "# of TileSetAssets: " << tilesetAssetObjects->nodesetval->nodeNr << '\n';
            
            totalObjects = audioAssetObjects->nodesetval->nodeNr
                         + mapAssetObjects->nodesetval->nodeNr
                         + fontAssetObjects->nodesetval->nodeNr
                         + videoAssetObjects->nodesetval->nodeNr
                         + tilesetAssetObjects->nodesetval->nodeNr;
            std::cout << "Total Asset Objects: " << totalObjects;

            
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

std::string StrataConfig::FindGameHash(boost::filesystem::path gamePath)
{
    boost::uuids::detail::sha1 myHash;
    std::stringstream finalHash;
    std::string hello = "Hello World";
    char hash[20];
    
    try
    {
        if(boost::filesystem::exists(gamePath))
        {
            if(boost::filesystem::is_regular_file(gamePath))
               {
                   std::cout << gamePath << " size of file " << boost::filesystem::file_size(gamePath) << '\n';
                   
               }
            else if(boost::filesystem::is_directory(gamePath))
            {
                std::cout << gamePath << " is a directory containing:\n";
            }
           
            std::copy(boost::filesystem::directory_iterator(gamePath), boost::filesystem::directory_iterator(),  // directory_iterator::value_type
                      std::ostream_iterator<boost::filesystem::directory_entry>(std::cout, "\n"));  // is directory_entry, which is
            // converted to a path by the
            // path stream inserter
        }
        else
            std::cout << gamePath << " exists, but is neither a regular file nor a directory\n";
        
    }
    catch (const boost::filesystem::filesystem_error& ex)
    {
        std::cout << ex.what() << '\n';
    }
    
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
}

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
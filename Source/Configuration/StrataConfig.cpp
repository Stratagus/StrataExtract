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
        configurationDocument = xmlReadFile(configurationPath.string().c_str(), NULL, 1);
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
            configurationRoot = xmlDocGetRootElement(configurationDocument);
            configXPathContext = xmlXPathNewContext(configurationDocument);
        }
        

        
        findGameEdition();
        
        
        //free the document
        //xmlFreeDoc(configurationDocument);
        //configurationDocument = NULL;
        
        //Free the global variables that may
        //have been allocated by the parser.
        //xmlCleanupParser();
        
        configLoaded = 1;
    }
}

void StrataConfig::findGameEdition()
{
    std::string gameHash = "3957d7ac483d7fbbd543e0cd70633bc9c57adfad";
    xmlXPathObjectPtr gameVersions = xmlXPathEval((const xmlChar *) "//Version", configXPathContext);
    std::cout << "Game Versions in Config: " << gameVersions->nodesetval->nodeNr << '\n';
    //std::cout << "First object in versions: " << gameVersions->nodesetval->nodeTab[1]->properties->next->name << '\n';

    //std::cout << "Property: " << xmlGetProp(gameVersions->nodesetval->nodeTab[1], (const xmlChar *) "//name") << '\n';
    //xmlNodeGetContent(<#xmlNodePtr cur#>)
    //std::cout << "XMLContent: " << xmlNodeGetContent(gameVersions->nodesetval->nodeTab[1])
    
    for(int numberOfVersions = 0; numberOfVersions < (gameVersions->nodesetval->nodeNr); numberOfVersions++)
    {
        xmlNodePtr currentNodePointer = gameVersions->nodesetval->nodeTab[numberOfVersions];
        xmlAttr* attribute = currentNodePointer->properties;
        while(attribute && attribute->name && attribute->children)
        {
            xmlChar* value = xmlNodeListGetString(currentNodePointer->doc, attribute->children, 1);
            std::cout << "Parent Property: " << value << '\n';
            //do something with value
            xmlFree(value);
            attribute = attribute->next;
        }
        
        
        
        std::cout << "Number of Children: " << xmlChildElementCount(currentNodePointer) << '\n';
        xmlNodePtr currentChildPointer = currentNodePointer->children->next;
        
        for(int numberOfFiles = 0; numberOfFiles < xmlChildElementCount(currentNodePointer); numberOfFiles++)
        {
            xmlAttr* childattribute = currentChildPointer->properties;
            while(childattribute && childattribute->name && childattribute->children)
            {
                xmlChar* childvalue = xmlNodeListGetString(currentChildPointer->doc, childattribute->children, 1);
                std::cout << "Childs Property: " << childvalue << '\n';
                //do something with value
                xmlFree(childvalue);
                childattribute = childattribute->next;
            }
            currentChildPointer = currentChildPointer->next;
        }
        //if(xmlChildElementCount(currentNodePointer) > 0)
        //{
        //    std::cout << "Child Content: " << currentNodePointer->children->next->name << '\n';
        //}
    }
    

    
    //print_xpath(gameVersions->nodesetval);
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

xmlChar* StrataConfig::GetFileHash(boost::filesystem::path filePath)
{
    //SHA1 hash here
    return NULL;
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

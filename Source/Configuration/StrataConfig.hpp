/*!
 *  \brief     Singleton Class responsible for Config setup/system queries.
 *  \details   
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      December 18, 2012
 *  \copyright GPLv2
 */

#ifndef StrataConfig_Header
#define StrataConfig_Header

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/exception/all.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/sha1.hpp>

#include <math.h>
#include <string>
#include <iterator>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#define VERSION 0.1f

class StrataConfig
{
    public:
        static StrataConfig *Configuration();
    
        //!Reads the Configuration XML
        /*!  Reads the Configuration XML
        *  \pre 
        *  \post
        *  \note*/
        void readConfig(boost::filesystem::path configurationPath);
    
        float GetProgess();
        bool isConfigLoaded();
    
    
        static void PrintUsage();
    //!Queries the number of CPU Cores.
    /*!  Queries the OS for the number of Core availible on
     *  the CPU.
     *  \pre None
     *  \post Return the number of availible cores
     *  \note*/
        static int GetCPUCores();
        //std::string FindSourcePathHash(boost::filesystem::path gamePath);

    void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);
    void print_xpath(xmlNodeSetPtr nodes);
    
    protected:
        StrataConfig();
    
        std::string getGameName();
    
        xmlNodePtr findGameEdition();
        void parseConfig();
    
    
        xmlChar *GetFileHash(boost::filesystem::path filePath);
    
        xmlNodePtr configurationRoot;
        xmlNodePtr gameVersionDetected;
        xmlXPathContextPtr configXPathContext;
    
        std::string *destinationImageFormat;
        std::string *destinationVideoFormat;

    private:
        static StrataConfig *ConfigurationInstance;
        int completeObjects;
        int totalObjects;
        bool configLoaded;
        int verbosity;
    
    
        std::string gameName;
};

#endif
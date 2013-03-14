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
#include <queue>

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
    
        //!Returns the current Progress
        /*!  
         *  \pre
         *  \post
         *  \note*/
        float GetProgess();
    
        //!Checks if a game version is loaded
        /*!
         *  \pre
         *  \post
         *  \note*/
        bool isConfigLoaded();
    
        void ProcessGameAssetLists();
    
        bool isExpansionGame();
    
        bool setGameMediaSourcePath(boost::filesystem::path gameMediaSourcePath);
        bool setGameMediaDestinationPath(boost::filesystem::path gameMediaDestinationPath);
    
        boost::filesystem::path GameMediaSourcePath();
        boost::filesystem::path GameMediaDestinationPath();
    
        static void PrintUsage();
    //!Queries the number of CPU Cores.
    /*!  Queries the OS for the number of Core availible on
     *  the CPU.
     *  \pre None
     *  \post Return the number of availible cores
     *  \note*/
        static int GetCPUCores();
        //std::string FindSourcePathHash(boost::filesystem::path gamePath);
    
    std::string getGameName();

    void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);
    void print_xpath(xmlNodeSetPtr nodes);
    
    protected:
        StrataConfig();
    
        
    
        xmlNodePtr FindGameVersion();
        void parseConfig();
        void ProcessArchive(xmlNodePtr archive);
    
    
        xmlChar *GetFileHash(boost::filesystem::path filePath);
    
        xmlNodePtr configurationRoot;
        xmlNodePtr gameVersion;
    
        std::string *destinationImageFormat;
        std::string *destinationVideoFormat;
    
        boost::filesystem::path gameMediaSource;
        boost::filesystem::path gameMediaDestination;
    
        xmlDoc *configurationDocument;
    
        xmlNodePtr LookupArchive(xmlChar* archiveName);
    
        std::queue<xmlNodePtr> preparationProcessQueue;
        std::queue<xmlNodePtr> processQueue;
        xmlXPathContextPtr configXPathContext;

    private:
        static StrataConfig *ConfigurationInstance;
        int completeObjects;
        int totalObjects;
        bool configLoaded;
        //Is the GameMediaSource a expansion disc?
        bool isExpansion;
        int verbosity;
    
    
        std::string gameName;
};

#endif
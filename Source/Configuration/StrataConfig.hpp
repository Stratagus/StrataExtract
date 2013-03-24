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
#include <boost/uuid/sha1.hpp>

#include <fstream>
#include <string>
#include <queue>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#define VERSION 0.1f

class StrataConfig
{
    public:
        static StrataConfig *Configuration();
    
        //!Reads the Configuration XML
        /*!  Reads the strataextract configuration to ensure it is correctly
         *  formated, otherwise throw/return false. Once the configuration is loaded
         *  the configuration searches for the proper version of the game to extract.
         *  (Based of the SHA1 hash)
         *  \pre gameMediaSource & configurationFilePath must be valid
         *  \post Parses the gameconfiguration and identifies the game version
         *  \note NA*/
        bool readConfig(boost::filesystem::path configurationPath);
    
        //!Returns the gameMedia extract progress
        /*!  
         *  \pre NA (Though for it to mean anything ProcessGameAssetLists() should be called
         *  to get values with any meaning)
         *  \post Returns the % progress of files being processed
         *  \note NA*/
        float GetProgess();
    
        //!Returns whether a game configuration has been read
        /*! Return true is a game configuration has been read and the 
         *  gameMediaSource has been correctly identified.
         *  \pre NA
         *  \post Returns true if the configuration has been loaded and
         *  identified, otherwise return false
         *  \note NA*/
        bool isConfigLoaded();
    
        //!Reads the loaded configuratio and creates the process queues
        /*!Creates the StrataRunner process queues in which to process,
         *  including creating the dependancy requirements.
         *  \pre Configuration must be loaded and identified correctly
         *  \post Creates the process queues for StrataRunner to process
         *  \note NA*/
        void ProcessGameAssetLists();
    
        //!Returns whether the game identified is a expansion edition
        /*Returns true if the game indentified is a expansion, else
         *  return false.
         *  \pre Configration must be loaded and game identified correctly
         *  \post Returns true if the game is a expansion else return false
         *  \note NA*/
        bool isExpansionGame();
    
        //!Sets the original game media file path.
        /*!Checks and sets the original game media file path allowing for the 
         *game edition to be identified
         *  \pre NA
         *  \post If the game path is valid then set the game path and return true,
         *  else return false
        *  \note NA*/
        bool setGameMediaSourcePath(boost::filesystem::path gameMediaSourcePath);
    
        //!Set the extracted+converted game media assets to the destination directory
        /*!Checks to ensure that the directory is writable and is acceptable to use for
         *  saving the extracted+converted media
         *  \pre Must be a valid file path location
         *  \post True if the filepath is writable and accessable, false otherwise
         *  \note*/
        bool setGameMediaDestinationPath(boost::filesystem::path gameMediaDestinationPath);
    
        //!Returns the current value of the GameMediaSourcePath
        /*!Returns a copy of the currently set, gameMediaSource filepath
         *  \pre NA
         *  \post Return filepath to the game media source.
         *  \note*/
        boost::filesystem::path GameMediaSourcePath();
    
        //!Returns the current game media destination filePath
        /*!Returns the current game media destination file path, the location
         *  of extracted and converted file for the Stratagus engine.
         *  \pre NA
         *  \post Returns a copy of the destination media filepath
         *  \note*/
        boost::filesystem::path GameMediaDestinationPath();
    
        //!Prints the proper CLI usage information
        /*Prints the CLI usage information allowing the user to reformat their
         *  command line/terminal arguments.
         *  \pre NA
         *  \post Prints the CLI help information to the terminal
         *  \note*/
        static void PrintUsage();
    
        //!Queries the number of CPU Cores.
        /*!  Queries the OS for the number of Core availible on
         *  the CPU.
         *  \pre None
         *  \post Return the number of availible cores
         *  \note*/
        static int GetCPUCores();
    
        //!Returns the game of the detected
        /*Returns the fulle name of the game identified in the Readconfiguration()
         * process.
         *  \pre The configuration must be read and properly Identified
         *  \post Returns a string with the name of the game identified.
         *  \note*/
        std::string getGameName();

    
    protected:
        //!The initialization & getter method
        /*!Initializes the default settings 
         * \pre NA
         * \post Creates a initial copy of the Strataconfig class
         *  otherwise return a pointer to the class instead
         * \note Singleton class implementation */
        StrataConfig();
    
        //!Identifies the game version
        /*!Identifies the game version based on the read configuration
         *  file, along with the game media source path 
         *  \pre The gameMediaFilePath must be set to a valid configuration
         *  \post Returns a pointer to the detected version of the gameAssets
         * \note NA*/
        xmlNodePtr FindGameVersion();
    
        //!Processes the assets for a given xmlNodePtr
        /*!Processes the assets for a given archive and add the assets nodeptrs
         *  onto the Stratarunner queue for processing
         * \pre The xmlNodePtr must contain a valid xmlNodePtr to a archive tag
         * \post Pushes the game assets from the archive onto the processing queues
         * \note NA*/
        void ProcessArchive(xmlNodePtr archive);
    
        //!Finds a archive based on the name of the archive
        /*!Finds the name of a specific archive tag to allow for 
         *  dependancy processing.
         *  \pre
         *  \post Returns xmlNodePtr to the archive being searched for,
         *       else return NULL
         *  \note NA*/
        xmlNodePtr LookupArchive(xmlChar* archiveName);
    
        //!Calculates the SHA1 a file from filePath
        /*!Calculate a SHA1 file checksum via Boost/OpenSSL to allow
         *  for checking if a matches the configuration SHA1
         *\pre NA
         *\post NA
         * \note NA*/
        xmlChar *GetFileHash(boost::filesystem::path filePath);
    
        //StrataConfig protected data members
        xmlDoc *configurationDocument;
        xmlNodePtr configurationRoot;
        xmlNodePtr gameVersion;
    
        boost::filesystem::path gameMediaSource;
        boost::filesystem::path gameMediaDestination;
    
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
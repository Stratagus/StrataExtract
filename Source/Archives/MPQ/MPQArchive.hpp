#ifndef MPQArchive_Header
#define MPQArchive_Header

#define MPQArchiveType

#include <boost/filesystem.hpp>
#include <stormlib.h>

class MPQArchive
{
    public:
        //!MPQArchive Constructor
        /*!  MPQArchive Object constructor
         *  \pre NA
         *  \post MPQArchive object is created and internal
         *        data structures set to NULL.
         *  \note NA*/
        MPQArchive();
        //!MPQArchive DeConstructor
        /*!  MPQArchive Object deconstructor
         *  \pre MPQArchive defined.
         *  \post MPQArchive object closed (if any) then deleted.
         *  \note NA*/
        ~MPQArchive();
    
        //!Create MPQ Archive
        /*!  Creates a new MPQArchive at the desired location
         *  \pre MPQArchive Defined
         *  \post Blank MPQArchive is created and written to disk
         *  \param[in] newArchivePath The file location in which to place the
         *                            new MPQArchive.
         *  \param[in] maxNumberOfFiles Sets the maximum amount of files that the
         *                              MPQArchive can hold at a given time.
         *                             
         *  \param[in] MPQArchiveVersion Set the MPQArchive version to create
         *                               MPQ_CREATE_ARCHIVE_V1 - 4GB (Starcraft,WarcraftII,Diablo,DiabloII,WarcraftIII)
         *                               MPQ_CREATE_ARCHIVE_V2 - 4GB+
         *                               MPQ_CREATE_ARCHIVE_V3 - 4GB+ (WoW-Cataclysm BETA)
         *                               MPQ_CREATE_ARCHIVE_V4 - 4GB+ (WoW-Cataclysm)
         *  \note NA*/
        void CreateArchive(boost::filesystem::path newArchivePath, int maxNumberOfFiles, DWORD MPQArchiveVersion);
        //!Open an existing MPQ Archive
        /*!  Open an existing MPQ Archive without a list file
         *  \pre MPQArchive Defined
         *  \post MPQArchive object will open MPQArchive specified
         *  \param[in] archivePath The file path to the MPQ Archive.
         *  \note NA*/
        void OpenArchive(boost::filesystem::path archivePath);
        //!Open an existing MPQ Archive
        /*!  Open an existing MPQ Archive with a list file
         *  \pre MPQArchive Defined
         *  \post MPQArchive object will open MPQArchive specified
         *  \param[in] archivePath The file path to the MPQ Archive
         *  \param[in] listPath The file path to the associated list file
         *  \note*/
        void OpenArchive(boost::filesystem::path archivePath, boost::filesystem::path listPath);
        //!Use Additional list files to MPQArchive
        /*!  Add additional list files to the MPQArchive (to acheive better search performance)
         *  \pre MPQArchive must already be open
         *  \post The list file will be included in MPQArchive operations
         *  \param[in] listFilePath The file path to the list file to add to the MPQArchive
         *  \note NA*/
        void ApplyListFile(boost::filesystem::path listFilePath);
        //!Read file from MPQArchive
        /*!  Read a file from the archive into memory
         *  \pre
         *  \post
         *  \param[in] src source describ
         *  \param[out] dest dest describ
         *  \note NOT IMPLEMENTED YET*/
        void *ReadFile(boost::filesystem::path archiveFilePath);
        //!Extract file to disk
        /*!  Simply extract a file to disk.
         *  \pre MPQArchive must be defined and open
         *  \post The file will be extracted to the destination location
         *  \param[in] archiveFilePath The relative path (in the archive) of the desired file
         *                             to extract. 
         *                             (eg. music/terran1.wav)-UNIX/POSIX (eg. music\terran1.wav)-WIN
         *  \param[in] destinationPath The on disk file path to extract to on the disk.
         *                             (eg. ./terran1.wav)-UNIX/POSIX (eg. C:\terran1.wav)-WIN
         *  \note NA*/
        void ExtractRaw(boost::filesystem::path archiveFilePath, boost::filesystem::path destinationPath);
        //!Close the MPQArchive
        /*!  Close the current MPQArchive.
         *  \pre MPQArchive must be open
         *  \post MPQArchive will be closed
         *  \note NA*/
        void CloseArchive();
    protected:
    
    private:
        HANDLE mpqArchive;
};

#endif
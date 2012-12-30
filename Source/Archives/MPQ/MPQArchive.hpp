#ifndef MPQArchive_Header
#define MPQArchive_Header

#define MPQArchiveType

#include <boost/filesystem.hpp>
#include <stormlib.h>

class MPQArchive
{
    public:
        MPQArchive();
        ~MPQArchive();
    
        void CreateArchive(boost::filesystem::path newArchivePath, int maxNumberOfFiles, DWORD MPQArchiveVersion);
        void OpenArchive(boost::filesystem::path archivePath);
        void OpenArchive(boost::filesystem::path archivePath, boost::filesystem::path listPath);
        void ApplyListFile(boost::filesystem::path listFilePath);
        void ExtractRaw(boost::filesystem::path archiveFilePath, boost::filesystem::path destinationPath);
        void CloseArchive();
    protected:
    
    private:
        HANDLE mpqArchive;
};

#endif
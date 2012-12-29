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
    
        void OpenArchive(boost::filesystem::path archivePath);
        void CreateArchive(boost::filesystem::path newArchivePath);
        void CloseArchive();
    protected:
    
    private:
        HANDLE mpqArchive;
};

#endif
#ifndef MPQArchive_Header
#define MPQArchive_Header

#include <boost/filesystem.hpp>
#include <stormlib.h>

class MPQArchive
{
    public:
        MPQArchive();
        ~MPQArchive();
    
        void OpenArchive(boost::filesystem::path archivePath);
        void CloseArchive();
    protected:
    
    private:
        HANDLE mpqArchive;
};

#endif
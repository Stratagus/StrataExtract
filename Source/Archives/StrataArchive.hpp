#ifndef StrataArchive_Header
#define StrataArchive_Header
#include "MPQ/MPQArchive.hpp"
#include "GZIP/GZIPArchive.hpp"

#include <libxml/parser.h>


class StrataArchive
{
    public:
        StrataArchive();
        ~StrataArchive();
        StrataArchive(xmlNodePtr archiveAsset);
        StrataArchive(boost::filesystem::path archiveFilePath);
        bool SetArchivePath(boost::filesystem::path archiveFilePath);
        bool IdentifyFileType();
        bool CreateArchive(boost::filesystem::path newArchiveFilePath);
        bool AddEntity(boost::filesystem::path archiveRelativeFilePath, boost::filesystem::path sourceFilePath);
        bool ExtractEntity(boost::filesystem::path archiveRelativeFilePath, boost::filesystem::path destinationFilePath);
        bool RemoveEntity(boost::filesystem::path archiveRelativeFilePath);
    protected:
    private:
};
#endif

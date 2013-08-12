#ifndef MPQArchiveException_Header
#define MPQArchiveException_Header

#include "../StrataArchiveException.hpp"
#include <boost/filesystem/path.hpp>

class MPQArchiveException : public StrataArchiveException
{
public:
    boost::filesystem::path *errorPath;
};

class MPQArchiveFailedToCreateArchive : public MPQArchiveException{};

class MPQArchiveFailedToOpenArchive : public MPQArchiveException{};


#endif
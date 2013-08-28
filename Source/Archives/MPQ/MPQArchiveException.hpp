#ifndef MPQArchiveException_Header
#define MPQArchiveException_Header

#include "../StrataArchiveException.hpp"
#include <boost/filesystem/path.hpp>

class MPQArchiveException : public StrataArchiveException
{
public:
    boost::filesystem::path *errorPath;
};

class MPQArchiveFailedToCreateArchive : public MPQArchiveException {};

class MPQArchiveFailedToOpenArchive : public MPQArchiveException {};

class MPQArchiveFailedToCloseArchive : public MPQArchiveException {};

class MPQArchiveFailedAlreadyOpen : public MPQArchiveException {};

class MPQArchiveFailedToOpenFile : public MPQArchiveException {};

class MPQArchiveFailedToCloseFile : public MPQArchiveException {};

class MPQArchiveFailedFileExtraction : public MPQArchiveException {};

class MPQArchiveNoArchiveOpen : public MPQArchiveException {};

class MPQArchiveNoFreeMemory : public MPQArchiveException {};



#endif
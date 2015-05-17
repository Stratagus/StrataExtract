#pragma once

#include "../StrataArchiveException.hpp"
#include <boost/filesystem/path.hpp>

class MPQArchiveException : public StrataArchiveException
{
public:
    boost::filesystem::path *errorPath = nullptr;
};

//Archive file operation errors
class MPQArchiveFailedToCreateArchive : public MPQArchiveException {};
class MPQArchiveFailedToOpenArchive : public MPQArchiveException {};
class MPQArchiveFailedToCloseArchive : public MPQArchiveException {};
class MPQArchiveFailedAlreadyOpen : public MPQArchiveException {};
class MPQArchiveNoArchiveOpen : public MPQArchiveException {};

//Archive file extraction operations
class MPQArchiveFailedToOpenFile : public MPQArchiveException {};
class MPQArchiveFailedToCloseFile : public MPQArchiveException {};
class MPQArchiveFailedFileExtraction : public MPQArchiveException {};
class MPQARchiveFailedToReadFile : public MPQArchiveException{};


//Listfile options
class MPQArchiveFailedtoOpenListfile : public MPQArchiveException {};

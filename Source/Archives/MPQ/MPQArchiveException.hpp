#ifndef MPQArchiveException_Header
#define MPQArchiveException_Header

#include "../StrataArchiveException.hpp"
#include <boost/filesystem/path.hpp>

class MPQArchiveException : public StrataArchiveException
{
public:
    boost::filesystem::path *errorPath = NULL;
};

class MPQArchiveFailedToCreateException : public MPQArchiveException{};

class MPQArchiveFailedToOpenException : public MPQArchiveException{};

class MPQArchiveAlreadyOpenException : public MPQArchiveException{};

class MPQArchiveNotOpenException : public MPQArchiveException{};

class MPQArchiveFailedToOpenFileException : public MPQArchiveException{};

class MPQARchiveFailedToReadFileException : public MPQArchiveException{};

class MPQArchiveFailedToCloseException : public MPQArchiveException{};

#endif
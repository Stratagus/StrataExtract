#include "MPQArchive.hpp"

void MPQArchive::OpenArchive(boost::filesystem::path archivePath)
{
    if (!SFileOpenArchive(archivePath.string().c_str(), 0, MPQ_OPEN_READ_ONLY, &mpqArchive))
    {
        std::cerr << "Failed to open the file '" << archivePath << "'\n";
        exit (-1);
    }
}

void MPQArchive::CloseArchive()
{
    SFileCloseArchive(mpqArchive);
}
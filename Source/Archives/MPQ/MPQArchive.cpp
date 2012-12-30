#include "MPQArchive.hpp"

MPQArchive::MPQArchive()
{
    mpqArchive = NULL;
}
MPQArchive::~MPQArchive()
{
    
}

//Creates archive
void MPQArchive::CreateArchive(boost::filesystem::path newArchivePath, int maxNumberOfFiles, DWORD MPQArchiveVersion)
{
    if(!SFileCreateArchive(newArchivePath.string().c_str(), MPQArchiveVersion, 1000, &mpqArchive))
    {
        throw "Failed to create archive";
    }
}

void MPQArchive::OpenArchive(boost::filesystem::path archivePath)
{
    if(mpqArchive)
    {
        throw "Archive already open";
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), 0, 0, &mpqArchive))
    {
        std::cerr << "Failed to open the file '" << archivePath << "'\n";
        exit (-1);
    }
    
}

void MPQArchive::OpenArchive(boost::filesystem::path archivePath, boost::filesystem::path listPath)
{
    if(mpqArchive)
    {
        throw "Archive already open";
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), MPQ_OPEN_NO_LISTFILE, MPQ_OPEN_FORCE_MPQ_V1, &mpqArchive))
    {
        std::cerr << "Failed to open the file '" << archivePath << "'\n";
        exit (-1);
    }
    
}

void MPQArchive::ApplyListFile(boost::filesystem::path listFilePath)
{
    if(!mpqArchive)
    {
        throw "Archive not opened";
    }
    else
    {
        SFileAddListFile(mpqArchive, listFilePath.string().c_str());
    }
}

void MPQArchive::ExtractRaw(boost::filesystem::path archiveFilePath, boost::filesystem::path destinationPath)
{
    if(!mpqArchive)
    {
        throw "Archive not opened";
    }
    else
    {
        if(!SFileExtractFile(&mpqArchive, archiveFilePath.string().c_str(), destinationPath.string().c_str(), SFILE_OPEN_FROM_MPQ))
        {
           throw "Failed to extract file";
        }
    }
}

void MPQArchive::CloseArchive()
{
    SFileCloseArchive(mpqArchive);
}
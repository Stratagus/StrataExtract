#include "MPQArchive.hpp"

MPQArchive::MPQArchive()
{
    mpqArchive = NULL;
}
MPQArchive::~MPQArchive()
{
    if(mpqArchive)
    {
        this->CloseArchive();
        mpqArchive = NULL;
    }
}

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
    
    std::cout << "Opening Archive: " << archivePath << " with Listfile: " << listPath << '\n';
    if(mpqArchive)
    {
        throw "Archive already open";
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), MPQ_OPEN_NO_LISTFILE, 0, &mpqArchive))
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

std::vector<char> *MPQArchive::ReadFile(boost::filesystem::path archiveFilePath)
{
    if(!mpqArchive)
    {
        throw "Archive not opened";

    }
    else
    {
        std::vector<char> *fileBuffer = new std::vector<char>;
        
        HANDLE fileHandle;
        if(!SFileOpenFileEx(mpqArchive, archiveFilePath.string().c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle))
        {
            throw "failed to open file in mpqArchive";
        }
        fileBuffer->resize(SFileGetFileSize(fileHandle, NULL));
        if (!SFileReadFile(fileHandle, &fileBuffer->at(0), SFileGetFileSize(fileHandle, NULL), NULL, NULL))
        {
            throw "failed to read file to memory";
        }
        if(!SFileCloseFile(fileHandle))
        {
            throw "failed to close file";
        }
        return fileBuffer;

    }
}


unsigned long MPQArchive::GetFileLength(boost::filesystem::path archiveFilePath)
{
    if(!mpqArchive)
    {
        throw "Archive not opened";
    }
    else
    {
        unsigned long fileLength;
        HANDLE fileHandle;
        if(!SFileOpenFileEx(mpqArchive, archiveFilePath.string().c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle))
        {
            throw "failed to open file in mpqArchive";
        }
         fileLength = SFileGetFileSize(fileHandle, NULL);
        if(!SFileCloseFile(fileHandle))
        {
            throw "could no close file";
        }
        return fileLength;
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
        std::cout << "Extract: " << archiveFilePath << " to destination: " << destinationPath << '\n';
        if(!SFileExtractFile(mpqArchive, archiveFilePath.string().c_str(), destinationPath.string().c_str(), SFILE_OPEN_FROM_MPQ))
        {
           throw "Failed to extract file";
        }
    }
}

void MPQArchive::CloseArchive()
{
    if (!mpqArchive)
    {
        throw "No MPQArchive to close";
    }
    else
    {
        SFileCloseArchive(mpqArchive);
        mpqArchive = NULL;
    }
}
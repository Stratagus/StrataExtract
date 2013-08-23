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
        MPQArchiveFailedToCreateException failedToCreate;
        failedToCreate.SetErrorMessage("Failed to create archive");
        failedToCreate.errorPath = &newArchivePath;
        throw failedToCreate;
    }
}

void MPQArchive::OpenArchive(boost::filesystem::path archivePath)
{
    if(mpqArchive)
    {
        MPQArchiveAlreadyOpenException alreadyOpen;
        alreadyOpen.SetErrorMessage("Archive already open");
        alreadyOpen.errorPath = &archivePath;
        throw alreadyOpen;
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), 0, 0, &mpqArchive))
    {
        MPQArchiveFailedToOpenException failedToOpen;
        failedToOpen.SetErrorMessage("Failed to open the file");
        failedToOpen.errorPath = &archivePath;
        throw failedToOpen;
    }
    
}

void MPQArchive::OpenArchive(boost::filesystem::path archivePath, boost::filesystem::path listPath)
{
    if(mpqArchive)
    {
        MPQArchiveAlreadyOpenException alreadyOpen;
        alreadyOpen.SetErrorMessage("Archive already open");
        alreadyOpen.errorPath = &archivePath;
        throw alreadyOpen;
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), MPQ_OPEN_NO_LISTFILE, 0, &mpqArchive))
    {
        MPQArchiveFailedToOpenException failedToOpen;
        failedToOpen.SetErrorMessage("Failed to open the file");
        failedToOpen.errorPath = &archivePath;
        throw failedToOpen;
    }
    
}

void MPQArchive::ApplyListFile(boost::filesystem::path listFilePath)
{
    if(!mpqArchive)
    {
        MPQArchiveNotOpenException notOpen;
        notOpen.SetErrorMessage("Archive not opened");
        throw notOpen;
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
        MPQArchiveNotOpenException notOpen;
        notOpen.SetErrorMessage("Archive not opened");
        notOpen.errorPath = &archiveFilePath;
        throw notOpen;

    }
    else
    {
        std::vector<char> *fileBuffer = new std::vector<char>;
        
        HANDLE fileHandle;
        if(!SFileOpenFileEx(mpqArchive, archiveFilePath.string().c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle))
        {
            MPQArchiveFailedToOpenFileException unableToOpen;
            unableToOpen.SetErrorMessage("failed to open file in mpqArchive");
            throw unableToOpen;
            
        }
        fileBuffer->resize(SFileGetFileSize(fileHandle, NULL));
        if (!SFileReadFile(fileHandle, &fileBuffer->at(0), SFileGetFileSize(fileHandle, NULL), NULL, NULL))
        {
            MPQARchiveFailedToReadFileException unableToRead;
            unableToRead.SetErrorMessage("failed to read file to memory");
            throw unableToRead;
        }
        if(!SFileCloseFile(fileHandle))
        {
            MPQArchiveFailedToCloseException failedToClose;
            failedToClose.SetErrorMessage("failed to close file");
            throw failedToClose;
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
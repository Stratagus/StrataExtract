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
        MPQArchiveFailedToCreateArchive failedToCreate;
        failedToCreate.SetErrorMessage("Failed to create archive");
        failedToCreate.errorPath = &newArchivePath;
        throw failedToCreate;
    }
}

void MPQArchive::OpenArchive(boost::filesystem::path archivePath)
{
    if(mpqArchive)
    {
        MPQArchiveFailedToOpenArchive failedOpenArchive;
        failedOpenArchive.SetErrorMessage("Archive already open");
        throw failedOpenArchive;
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), 0, 0, &mpqArchive))
    {
        MPQArchiveFailedToOpenArchive failedToOpen;
        failedToOpen.SetErrorMessage("Failed to open the file");
        failedToOpen.errorPath = &archivePath;
        throw failedToOpen;
    }
    
}

void MPQArchive::OpenArchive(boost::filesystem::path archivePath, boost::filesystem::path listPath)
{
    if(mpqArchive)
    {
        MPQArchiveFailedAlreadyOpen alreadyOpen;
        alreadyOpen.SetErrorMessage("Archive already open");
        alreadyOpen.errorPath = &archivePath;
        throw alreadyOpen;
    }
    if(!SFileOpenArchive(archivePath.string().c_str(), MPQ_OPEN_NO_LISTFILE, 0, &mpqArchive))
    {
        MPQArchiveFailedToOpenArchive failedArchiveOpen;
        failedArchiveOpen.SetErrorMessage("Failed to open the specified archive");
        failedArchiveOpen.errorPath = &archivePath;
        throw failedArchiveOpen;
    }
    
}

void MPQArchive::ApplyListFile(boost::filesystem::path listFilePath)
{
    if(!mpqArchive)
    {
        MPQArchiveNoArchiveOpen noArchiveOpen;
        noArchiveOpen.SetErrorMessage("No MPQ Archive open");
        throw noArchiveOpen;
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
        MPQArchiveNoArchiveOpen noArchiveOpen;
        noArchiveOpen.SetErrorMessage("No MPQ Archive open");
        throw noArchiveOpen;
    }
    else
    {
        std::vector<char> *fileBuffer = new std::vector<char>;
        
        HANDLE fileHandle;
        if(!SFileOpenFileEx(mpqArchive, archiveFilePath.string().c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle))
        {
            MPQArchiveFailedToOpenFile failedFileOpen;
            failedFileOpen.SetErrorMessage("No file found in MPQ Archive");
            throw failedFileOpen;
        }
        fileBuffer->resize(SFileGetFileSize(fileHandle, NULL));
        if (!SFileReadFile(fileHandle, &fileBuffer->at(0), SFileGetFileSize(fileHandle, NULL), NULL, NULL))
        {
            MPQArchiveNoFreeMemory noMemory;
            noMemory.SetErrorMessage("Not enough memory to load target file");
            throw noMemory;
        }
        if(!SFileCloseFile(fileHandle))
        {
            MPQArchiveFailedToCloseFile failedFileClose;
            failedFileClose.SetErrorMessage("Failed to close file in archive");
            throw failedFileClose;
        }
        return fileBuffer;

    }
}


unsigned long MPQArchive::GetFileLength(boost::filesystem::path archiveFilePath)
{
    if(!mpqArchive)
    {
        MPQArchiveNoArchiveOpen noArchiveOpen;
        noArchiveOpen.SetErrorMessage("No archive open");
        throw noArchiveOpen;
    }
    else
    {
        unsigned long fileLength;
        HANDLE fileHandle;
        if(!SFileOpenFileEx(mpqArchive, archiveFilePath.string().c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle))
        {
            MPQArchiveFailedToOpenFile failedFileOpen;
            failedFileOpen.SetErrorMessage("No file found in MPQ Archive");
            throw failedFileOpen;
        }
         fileLength = SFileGetFileSize(fileHandle, NULL);
        if(!SFileCloseFile(fileHandle))
        {
            MPQArchiveFailedToCloseFile failedFileClose;
            failedFileClose.SetErrorMessage("Failed to close file in archive");
            throw failedFileClose;
        }
        return fileLength;
    }
}

void MPQArchive::ExtractRaw(boost::filesystem::path archiveFilePath, boost::filesystem::path destinationPath)
{
    if(!mpqArchive)
    {
        MPQArchiveNoArchiveOpen noArchiveOpen;
        noArchiveOpen.SetErrorMessage("No MPQ Archive open");
        throw noArchiveOpen;
    }
    else
    {
        if(!SFileExtractFile(mpqArchive, archiveFilePath.string().c_str(), destinationPath.string().c_str(), SFILE_OPEN_FROM_MPQ))
        {
            MPQArchiveFailedFileExtraction fileExtractionFailed;
            fileExtractionFailed.SetErrorMessage("Failed to extract file from MPQArchive");
            throw fileExtractionFailed;
        }
    }
}

void MPQArchive::CloseArchive()
{
    if (!mpqArchive)
    {
        MPQArchiveFailedToCloseArchive failedCloseArchive;
        failedCloseArchive.SetErrorMessage("Failed to close MPQArchive");
        throw failedCloseArchive;
    }
    else
    {
        SFileCloseArchive(mpqArchive);
        mpqArchive = NULL;
    }
}
#include "StrataConfigException.hpp"

std::string StrataConfigException::GetErrorMessage()
{
    return humanReadableError;
}

void StrataConfigFilesystemException::SetErrorMessage(std::string newMessage)
{
    humanReadableError = newMessage;
}
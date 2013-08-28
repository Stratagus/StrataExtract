#include "StrataArchiveException.hpp"
void StrataArchiveException::SetErrorMessage(const std::string &errorMessage)
{
    humanReadableError = "StrataConfig: ";
    humanReadableError.append(errorMessage);
}

std::string StrataArchiveException::GetErrorMessage()
{
    return humanReadableError;
}
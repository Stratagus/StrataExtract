#include "StrataConfigException.hpp"
void StrataConfigException::SetErrorMessage(const std::string &errorMessage)
{
    humanReadableError = "StrataConfig: ";
    humanReadableError.append(errorMessage);
}

std::string StrataConfigException::GetErrorMessage()
{
    return humanReadableError;
}

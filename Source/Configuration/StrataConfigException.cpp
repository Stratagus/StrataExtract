#include "StrataConfigException.hpp"

std::string StrataConfigException::GetErrorMessage()
{
    return humanReadableError;
}
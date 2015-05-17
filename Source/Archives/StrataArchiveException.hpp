#pragma once

#include <boost/exception/all.hpp>
#include <boost/filesystem/path.hpp>


//!The base exception for which all StrataConfig exceptions are based off of
/*!A base exception class
 * \pre NA
 * \post NA
 * \note */
class StrataArchiveException : public virtual std::exception, public virtual boost::exception
{
    
public:
    void SetErrorMessage(const std::string &errorMessage);
    
    ~StrataArchiveException() throw() {}
    virtual std::string GetErrorMessage();
protected:
    std::string humanReadableError;
};

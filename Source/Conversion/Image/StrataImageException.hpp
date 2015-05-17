#pragma once

#include <boost/exception/all.hpp>

//!The base exception for which all StrataImage exceptions are based off of
/*!A base exception class
 * \pre NA
 * \post NA
 * \note */
class StrataImageException : public virtual std::exception, public virtual boost::exception
{
    
public:
    void SetErrorMessage(const std::string &errorMessage);
    
    ~StrataImageException() throw() {}
    virtual std::string GetErrorMessage();
protected:
    std::string humanReadableError;
};
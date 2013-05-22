#ifndef StrataRunnerException_Header
#define StrataRunnerException_Header

#include <boost/exception/all.hpp>

//!The base exception for which all StrataConfig exceptions are based off of
/*!A base exception class
 * \pre NA
 * \post NA
 * \note */
class StrataRunnerException : public virtual std::exception, public virtual boost::exception
{
    
public:
    void SetErrorMessage(const std::string &errorMessage);
    
    ~StrataRunnerException() throw() {}
    virtual std::string GetErrorMessage();
protected:
    std::string humanReadableError;
};

#endif
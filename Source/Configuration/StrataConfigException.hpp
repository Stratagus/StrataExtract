#ifndef StrataConfigException_Header
#define StrataConfigException_Header

#include <boost/exception/all.hpp>
#include <boost/filesystem/path.hpp>


//!The base exception for which all StrataConfig exceptions are based off of
/*!A base exception class
 * \pre NA
 * \post NA
 * \note */
class StrataConfigException : public virtual std::exception, public virtual boost::exception
{
    
public:
    void SetErrorMessage(const std::string &errorMessage);
    
    ~StrataConfigException() throw() {}
    virtual std::string GetErrorMessage();
protected:
    std::string humanReadableError;
};

//!An exception
/*!A Exception
 * \pre NA
 * \post NA
 * \note */
class StrataConfigParsingException : public StrataConfigException {};

//!An exception
/*!A Exception
 * \pre NA
 * \post NA
 * \note */
class StrataConfigNotLoadedException : public StrataConfigException {};

//!An exception
/*!A Exception
 * \pre NA
 * \post NA
 * \note */
class StrataConfigFilesystemException : public StrataConfigException
{
    public:
        boost::filesystem::path *problemPath;
    
};
#endif
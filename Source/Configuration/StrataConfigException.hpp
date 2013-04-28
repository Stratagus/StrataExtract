#ifndef StrataConfigException_Header
#define StrataConfigException_Header

#include <boost/exception/all.hpp>
#include <boost/filesystem/path.hpp>


//!The base exception
/*!A Exception
 * \pre NA
 * \post NA
 * \note */
class StrataConfigException : public virtual std::exception, public virtual boost::exception
{
    
public:

    virtual std::string GetErrorMessage();
protected:
    ~StrataConfigException() throw() {}
    std::string humanReadableError;
};


//!An exception
/*!A Exception
 * \pre NA
 * \post NA
 * \note */
class StrataConfigParsingException : public StrataConfigException
{
    public:
        StrataConfigParsingException();
        StrataConfigParsingException(const std::string &errorMessage)
        {
            humanReadableError = errorMessage;
        }

};

//!An exception
/*!A Exception
 * \pre NA
 * \post NA
 * \note */
class StrataConfigFilesystemException : public StrataConfigException
{
    public:
        boost::filesystem::path *problemPath;
        void SetErrorMessage(std::string newMessage);
    
};
#endif
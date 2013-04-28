#ifndef StrataConfigException_Header
#define StrataConfigException_Header

#include <boost/exception/all.hpp>
#include <boost/filesystem/path.hpp>

class StrataConfigException : public virtual std::exception, public virtual boost::exception
{
    
public:

    virtual std::string GetErrorMessage();
protected:
    ~StrataConfigException() throw() {}
    std::string humanReadableError;
};



class StrataConfigParsingException : public StrataConfigException
{
    public:
        StrataConfigParsingException();
        StrataConfigParsingException(const std::string &errorMessage)
        {
            humanReadableError = errorMessage;
        }

};


class StrataConfigFilesystemException : public StrataConfigException
{
    public:
        boost::filesystem::path *problemPath;
        void SetErrorMessage(std::string newMessage);
    
};
#endif
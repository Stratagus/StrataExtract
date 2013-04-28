#ifndef StrataConfigException_Header
#define StrataConfigException_Header

#include <boost/exception/all.hpp>

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
    
};
#endif
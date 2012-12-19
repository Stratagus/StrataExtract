#ifndef StrataConfig_Header
#define StrataConfig_Header

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>
#include <boost/exception/all.hpp>
#include <math.h>



class StrataConfig
{
    public:
        static StrataConfig *Configuration();
        void readConfig(boost::filesystem::path configurationPath);
        static int GetCPUCores();
        float GetProgess();
    
    protected:
        StrataConfig();

    private:
        static StrataConfig* ConfigurationInstance;
        int completeObjects;
        int totalObjects;
};

#endif
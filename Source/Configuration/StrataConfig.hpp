/*!
 *  \brief     Singleton Class responsible for Config setup/system queries.
 *  \details   
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      December 18, 2012
 *  \copyright GPLv2
 */

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
    
        //!Reads the Configuration XML
        /*!  Reads the Configuration XML
        *  \pre 
        *  \post
        *  \note*/
        void readConfig(boost::filesystem::path configurationPath);
    
        //!Queries the number of CPU Cores.
        /*!  Queries the OS for the number of Core availible on
         *  the CPU.
         *  \pre None
         *  \post Return the number of availible cores
         *  \note*/
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
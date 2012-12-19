#ifndef StrataRunner_Header
#define StrataRunner_Header

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>
#include <boost/exception/all.hpp>

#include "../Configuration/StrataConfig.hpp"

class StrataRunner
{
    public:
        static StrataRunner *Runner();
        void run();
    protected:
        StrataRunner();
    private:
        static StrataRunner *runnerInstance;
        StrataConfig *config;
};

#endif
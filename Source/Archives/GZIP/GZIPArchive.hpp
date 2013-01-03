#ifndef GZIPArchive_Header
#define GZIPArchive_Header

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/filter/gzip.hpp>


#define GZIPArchiveType
class GZIPArchive
{
    public:
    GZIPArchive();
    ~GZIPArchive();
    void CreateArchive(boost::filesystem::path archivePath);
    protected:
    private:
};

#endif

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

void display_memlock()
{
    std::string filename;
    std::string keyword("VmLck");
    std::string line;

    filename.append("/proc/");
    filename.append(std::to_string(::getpid()));
    filename.append("/status");

    std::ifstream in(filename.c_str());
    if( in.is_open())
    {
        while( getline(in,line) )
        {
            if( line.find(keyword) != std::string::npos )
            {
                std::cerr <<"memory_map: kernel interface (" << filename << ") returns: " << line << std::endl;
            }
        }
    }
}

int fmtBytes(long long val, char *buff, int bsz)
{
    static const long long Kval = 1024LL;
    static const long long Mval = 1024LL*1024LL;
    static const long long Gval = 1024LL*1024LL*1024LL;
    static const long long Tval = 1024LL*1024LL*1024LL*1024LL;
    char sName = ' ';
    int resid;

// Get correct scaling
//
    if (val < 1024)  return snprintf(buff, bsz, "%lld", val);
    if (val < Mval) {
        val = val*10/Kval;
        sName = 'K';
    }
    else if (val < Gval) {
        val = val*10/Mval;
        sName = 'M';
    }
    else if (val < Tval) {
        val = val*10/Gval;
        sName = 'G';
    }
    else                 {
        val = val*10/Tval;
        sName = 'T';
    }
    resid = val%10LL;
    val = val/10LL;

// Format it
//
    return snprintf(buff, bsz, "%lld.%d%c", val, resid, sName);
}

void Usage(const char *msg)
{
    if (msg) std::cerr <<"memory_map: " << msg << std::endl;
    std::cerr <<"Usage: memory_map [-i] <filename>" << std::endl;
    exit (msg ? 1 : 0);
}

int main(int argc, char **argv)
{
    struct stat buf;
    char *theFile, *theArg = argv[1];
    int fd;
    char crB, buff[80];
    bool dowait = true;

// Make sure that we have a file name
//
    if (argc < 2) Usage("filename not specified.");

// See if there is an option
//
    if (!strcmp(theArg, "-i")) {
        dowait = false;
        theArg = argv[2];
    }

// Open the file and get its size
//
    if ((fd = open(theArg, O_RDONLY)) < 0 || fstat(fd, &buf) < 0)
    {   std::cerr <<"memory_map: " <<strerror(errno) <<" opening " << theArg << std::endl;
        exit(8);
    }

// Now memory map the file
//
    theFile = (char *)mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (theFile == (char *)-1)
    {   std::cerr <<"memory_map: mmap " <<argv[1] <<" failed; " <<strerror(errno) << std::endl;
        exit(8);
    }

// Now close the file descriptor
//
    close(fd);

// Indicate what we did
//
    fmtBytes(buf.st_size, buff, sizeof(buff));
    std::cerr <<"memory_map: " <<buf.st_size <<" bytes (" <<buff <<") mapped" << std::endl;

    display_memlock();

// Wait for this to complete if so wanted
//
    if (dowait)
    {
        std::cerr <<"memory_map: Hit enter to finish up... ";
        std::cin.get();
    }


// All done
//
    exit(0);
}


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>

#include "exception.h"



class filefd {
    private:
        FILE* fp;
        std::string name;
    public:
        filefd() : fp(NULL) {}
        ~filefd() 
        {
            close();
        }
        std::string& get_name()
        {
            return name;
        }
        void open(const char* path, const char* mode)
        {
            if (fp)
                close();
            fp = ::fopen(path, mode);
            if (!fp) {
                perror("fopen");
                exit(-1);
            }
            name = path;
        }
        void reopen(const char* path, const char* mode)
        {
            fp = ::freopen(path, mode, fp);
            if (!fp) {
                perror("fopen");
                exit(-1);
            }
            name = path;
        }
        void close()
        {
            if (fp)
                ::fclose(fp);
            name = "";
        }
        void write(const void* ptr, size_t size, size_t nmemb)
        {
            size_t res = ::fwrite(ptr, size, nmemb, fp);
            if (res != nmemb) {
                perror("fwrite");
                exit(-1);
            }
        }
        void write_imediate(const void* ptr, size_t size, size_t nmemb)
        {
            write(ptr, size, nmemb);
            flush();
        }
        size_t read(void* ptr, size_t size, size_t nmemb)
        {
            size_t res = ::fread(ptr, size, nmemb, fp);
            if (res != nmemb) {
                if (errno == 0)
                    return res;
                else
                    perror("fread");
            }
            return res;
        }
        void flush()
        {
            int res = ::fflush(fp);
            if (res == EOF) {
                perror("fflush");
                exit(-1);
            }
        }
        template<typename... ARG>
        void printf(const char* fmt, const ARG&... arg)
        {
            ::fprintf(fp, fmt, arg...);
        }
        template<typename... ARG>
        void printf_imediate(const char* fmt, const ARG&... arg)
        {
            printf(fmt, arg...);
            flush();
        }
};

    

template <class Tag>
class log {
    private:
        static bool inited;
        filefd fd;
        std::vector<std::string> depth;
    
        log() {}
        ~log() {}
        log(const log&) = delete;
        log& operator=(const log&) = delete; 

    public:
        static log& instance()
        {
            static log l;
            return l;
        }
        void open(const char* path)
        {
            if (inited)
                throw wmexception("Inited yet");
            inited = true;
            fd.open(path, "a");
        }
        void open_new(const char* path)
        {
            if (inited)
                throw wmexception("Inited yet");
            inited = true;
            fd.open(path, "w");
        }
        void clean()
        {
            fd.reopen(fd.get_name().c_str(), "w");
        }
        template<typename ... ARG>
        void write(const char* fmt, const ARG&... arg)
        {
            if (!inited)
                throw wmexception("Not inited yes");
            fd.printf(fmt, arg...);
            fd.printf("\n");
            fd.flush();
        }
};
template <class Tag>
bool log<Tag>::inited = false;


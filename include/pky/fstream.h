#pragma once

#include <stdio.h>
#include <string>

namespace pky
{
    class ofstream
    {
    public:
        ofstream(const std::string& path, bool appendMode = false);
        ofstream(const char* path, bool appendMode = false);
        ~ofstream();

        void open(const char* path, const char* mode);
        void close();
        void write(const std::string& buffer);
        void write(const char* buffer);

        ofstream& operator<<(const std::string& buffer);
    private:
        FILE* m_File = nullptr;
    };
}

#include <iostream>

#include "pky_fstream.h"
#include "pky_help.h"

namespace pky
{
    ofstream::ofstream(const std::string& file, bool appendMode)
    {
        std::cout << "const std::string& constructor\n\n";
        open(file.c_str(), appendMode ? "ab" : "wb");
    }

    ofstream::ofstream(const char* file, bool appendMode)
    {
        std::cout << "const char* constructor\n\n";
        open(file, appendMode ? "ab" : "wb");
    }

    ofstream::~ofstream()
    {
        fclose(m_File);
        m_File = nullptr;
    }

    void ofstream::open(const char* path, const char* mode)
    {
        if (m_File) fclose(m_File);
        m_File = fopen(path, mode);
    }

    void ofstream::write(const std::string& buffer)
    {
        if(m_File) fwrite(buffer.c_str(), sizeof(char), buffer.length(), m_File);
    }

    void ofstream::write(const char* buffer)
    {
        if (m_File) fwrite(buffer, sizeof(char), _strlen(buffer), m_File);
    }

    void ofstream::close()
    {
        fclose(m_File);
        m_File = nullptr;
    }

    ofstream& ofstream::operator<<(const std::string& buffer)
    {
        write(buffer.c_str());

        return *this;
    }
}
#include "pky/fstream.h"
#include "pky/help.h"

namespace pky
{
    ofstream::ofstream(const std::string& file, bool appendMode)
    {
        open(file.c_str(), "wb");
    }

    ofstream::ofstream(const char* file, bool appendMode)
    {
        open(file, "wb");
    }

    ofstream::~ofstream()
    {
        close();
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

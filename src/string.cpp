#include <stdexcept>
#include <format>
#include <string.h>
#include <iostream>

#include "pky/string.h"
#include "pky/help.h"

namespace pky
{
    /* TODO:
     *   - add sso
     *   - improve move semantics
     *   - implement more std::string methods
     */
    string::string()
    {
        m_Length = 0;
        m_Data = new char[1];
        m_Data[0] = '\0';
    }

    string::string(const char* ptr)
    {
        m_Length = _strlen(ptr);
        m_Data = new char[m_Length + 1];
        _memcpy(m_Data, ptr, m_Length);
    }

    string::string(const string& other) noexcept
        : m_Length(other.m_Length)
        , m_Data(new char[other.m_Length + 1])
    {
        _memcpy(m_Data, other.m_Data, m_Length);
    }

    string& string::operator=(string&& old) noexcept
    {
        if (this != &old) {
            // set current string
            m_Length = old.m_Length;
            delete[] m_Data;
            m_Data = old.m_Data;

            // invalidate old string
            old.m_Length = 0;
            // may fix some issues, but keep current implementation
            old.m_Data = new char[1];
            old.m_Data[0] = '\0';
        }
        return *this;
    }

    string::~string()
    {
        clear();
    }

    void string::clear()
    {
        if (m_Data != nullptr || strcmp(m_Data, "") != 0)
        {
            m_Length = 0;
            delete[] m_Data;
            m_Data = new char[1];
            m_Data[0] = '\0';
        }
    }

    void string::resize(size_t new_size)
    {
        char* buf = new char[new_size + 1];
        _memcpy(buf, m_Data, new_size);
        for(size_t i{m_Length}; i < new_size+1; i++)
            buf[i] = '\0';
        m_Data = buf;
        m_Length = new_size;
        m_Data[m_Length] = '\0';
    }

    char& string::at(size_t index)
    {
        if (index > m_Length)
            throw std::out_of_range(std::format("pky::string::at - argument `index` (which is {}) > this->length() (which is {})", index, m_Length));

        return m_Data[index];
    }

    string& string::append(const char* str)
    {
        _strcat(m_Data, str);
        m_Length = _strlen(m_Data);
        return *this;
    }

    string string::substr(size_t pos, size_t len)
    {
        string res;
        if(pos > m_Length)
            throw std::out_of_range(std::format("pky::string::substr - argument `pos` (which is {}) > this->length() (which is {})", pos, m_Length));

        // clamp len to, at max, the full size of the string that is calling this method
        if(len > m_Length)
            len = m_Length;
        
        res.m_Length = len;
        res.m_Data = new char[len + 1];
        
        _memcpy(res.m_Data, m_Data + pos, len);
        res.m_Data[len] = '\0';
        return res;
    }

    // TODO: find a way to avoid 4 _memcpy calls
    string& string::insert(size_t index, const char* str)
    {
        size_t newlen = m_Length + _strlen(str);
        resize(newlen);

        char* buf = new char[newlen + 1];
        _memcpy(buf, m_Data, index);
        _memcpy(buf + index, str, _strlen(str) + index);
        _memcpy(buf + index + _strlen(str), m_Data + index, m_Length);
        buf[m_Length] = '\0';
        _memcpy(m_Data, buf, m_Length);

        return *this;
    }

    size_t string::length() const { return m_Length; }
    char* string::c_str() const { return m_Data; }

    // operators
    char& string::operator[](size_t index)
    {
        return m_Data[index];
    }

    string& string::operator+=(const char* s)
    {
        return append(s);
    }

    std::ostream& operator<<(std::ostream& stream, const string& mystring)
    {
        stream << mystring.m_Data;
        return stream;
    }
}

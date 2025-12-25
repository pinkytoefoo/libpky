#pragma once

#include <stdint.h>
#include <ostream>

namespace pky
{
    /*
     * TODO:
     *   - add sso
     *   - improve move semantics
     *   - add std::cout functionality
     *   - implement more std::string methods
     */
    class string
    {
    public:
        string();
        string(const char* ptr);
        string(const string& old) noexcept;
        string& operator=(string&& other) noexcept;
        ~string();

        void clear();
        char& at(uint32_t index);
        string& append(const char* str);
        string& insert(uint32_t index, const char* str);

        uint32_t length() const;
        char* c_str() const;

        // operators
        char& operator[](uint32_t index);
        string& operator+=(const char* str);

        friend std::ostream& operator<<(std::ostream& stream, const string& mystring);
    private:

        uint32_t m_Length;
        char* m_Data;
    };
}
#pragma once

#include <stdint.h>
#include <ostream>
#include <string>

namespace pky
{
    /*
     * TODO:
     *   - add sso
     *   - improve move semantics
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
        void resize(size_t new_size);
        char& at(size_t index);
        string& append(const char* str);
        string& insert(size_t index, const char* str);
        string substr(size_t pos, size_t len = size_t{-1uz}); // messing with brace initilization and c++23 integer literals (uz suffix)

        size_t length() const;
        char* c_str() const;

        // operators
        char& operator[](size_t index);
        string& operator+=(const char* str);

        friend std::ostream& operator<<(std::ostream& stream, const string& mystring);
    private:
        size_t m_Length;
        char* m_Data;
    };
}
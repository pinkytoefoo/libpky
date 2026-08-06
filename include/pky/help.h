#pragma once

#include <stdint.h>

namespace pky
{
    uint32_t _strlen(const char* ptr);
    void _memcpy(void* dest, const void* src, size_t count);
    char* _strcat(char* dest, const char* add);

    #if __cplusplus >= 202302L
    template<typename T1, typename T2>
    class _compressed_pair
    {
    public:
        _compressed_pair(T1 t1, T2 t2)
            : T1(std::move(t1))
            , T2(std::move(t2))
        {
        }

        T1& first() { return first; }
        T2& second() { return second_; }
        const T1& first() const { return first_; }
        const T2& second() const { return second_; }

    private:
        [[no_unique_address]] T1 first_;
        T2 second_;
    };
    #else
    template<typename T1, typename T2, bool = std::is_empty_v<T1>>
    class _compressed_pair : public T1
    {
    public:
        _compressed_pair(T1 t1, T2 t2)
            : T1(std::move(t1))
            , T2(std::move(t2))
        {
        }

        T& first() { return *this; }
        T& second() { return second_; }
        const T& first() const { return *this; }
        const T& second() const { return second_; }


    private:
        T2 second_;
    };
    #endif
}
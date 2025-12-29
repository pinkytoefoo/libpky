#include <stdint.h>
#include <iostream>

#include "pky/help.h"

namespace pky
{
    uint32_t _strlen(const char* ptr)
    {
        if (ptr == nullptr)
            return 0;

        uint32_t len = 0;
        for (; ptr[len] != '\0'; len++);
        return len;
    }

    // takes into account null terminator
    // only needs to pass the length as the # of chars
    void _memcpy(void* dest, const void* src, size_t count)
    {
        char* cdest = (char*)dest;
        const char* csrc = (const char*)src;

        for (size_t i = 0; i < count + 1; i++)
        {
            cdest[i] = csrc[i];
        }
    }

    // void _memcpy_nt(void* dest, const void* src, size_t count)
    // {
    //     char* cdest = (char*)dest;
    //     const char* csrc = (const char*)src;

    //     for (uint32_t i = 0; i < count + 1; i++)
    //     {
    //         // bug here somewhere
    //         cdest[i] = csrc[i];
    //         if(!csrc[i])
    //             cdest[i] = '\0';
    //     }
    // }

    // TODO: fix fragility and static analysis warning
    char* _strcat(char* dest, const char* add)
    {
        size_t destlength = _strlen(dest);
        size_t newlength = destlength + _strlen(add);
        char* res = new char[newlength + 1];

        for (size_t i = 0; i < newlength; i++)
        {
            if (i < destlength)
                res[i] = dest[i];
            else
                res[i] = add[i - destlength];
        }

        res[newlength] = '\0';
        return res;
    }
}

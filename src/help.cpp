#include <stdint.h>

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
    void _memcpy(void* dest, const void* src, uint32_t charCount)
    {
        char* cdest = (char*)dest;
        const char* csrc = (const char*)src;

        for (uint32_t i = 0; i < charCount + 1; i++)
        {
            cdest[i] = csrc[i];
        }
    }

    // TODO: fix fragility and static analysis warning
    char* _strcat(char* dest, const char* add)
    {
        uint32_t destlength = _strlen(dest);
        uint32_t addlength = _strlen(dest);
        uint32_t newlength = destlength + addlength;
        char* res = new char[newlength + 1];

        for (uint32_t i = 0; i < newlength; i++)
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

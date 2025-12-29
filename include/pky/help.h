#pragma once

#include <stdint.h>

namespace pky
{
    uint32_t _strlen(const char* ptr);
    void _memcpy(void* dest, const void* src, uint32_t count);
    // fills empty space with null terminators
    void _memcpy_nt(void* dest, const void* src, uint32_t count);
    char* _strcat(char* dest, const char* add);
}
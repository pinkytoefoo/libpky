#pragma once

#include <stdint.h>

namespace pky
{
    uint32_t _strlen(const char* ptr);
    void _memcpy(void* dest, const void* src, size_t count);
    char* _strcat(char* dest, const char* add);
}
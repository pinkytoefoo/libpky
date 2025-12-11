#pragma once

#include <stdint.h>

namespace pky
{
	uint32_t _strlen(const char* ptr);
	void _memcpy(void* dest, const void* src, uint32_t charCount);
	char* _strcat(char* dest, const char* add);
}
#pragma once

#include "pkystring.h"
#include "pkyhelp.h"

#ifdef TRACK_ALLOCATIONS
// https://en.cppreference.com/w/cpp/memory/new/operator_new.html
void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes\n";
	void* ptr = std::malloc(size);
	if (ptr == nullptr)
	{
		// throws exception, inline with C++ ISO standards
		// https://stackoverflow.com/questions/7194127/how-should-i-write-iso-c-standard-conformant-custom-new-and-delete-operators#7194137
		throw std::bad_alloc{};
	}
	return ptr;
}

// https://en.cppreference.com/w/cpp/memory/new/operator_delete
void operator delete(void* memory, size_t size) noexcept
{
	std::cout << "Freeing " << size << " bytes\n";

	free(memory);
}

void operator delete[](void* memory)
{
	std::cout << "Freeing unknown bytes\n";

	free(memory);
}
#endif

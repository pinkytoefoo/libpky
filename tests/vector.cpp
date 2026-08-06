#include <doctest.h>

#include "pky/vector.h"

TEST_SUITE_BEGIN("vector");

template<typename T>
class tracking_allocator
{
public:
    using value_type = T;
    using pointer = value_type*;

public:
    [[nodiscard]] pointer allocate(size_t count)
    {
        size_t byte_count = count * sizeof(value_type);
        std::cout << "tracking_allocator: allocating " << byte_count << " bytes\n";
        ::operator new(static_cast<pointer>(count * sizeof(value_type)));
    }
    
    template<typename... Args>
    void construct(pointer ptr, )

    void deallocate(pointer ptr)
    {
        ::operator delete(ptr);
    }

private:
};

TEST_CASE("")
{

}

TEST_SUITE_END();


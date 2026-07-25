#pragma once

#include <initializer_list>
namespace pky
{
    template<typename T>
    class vector
    {
    public:
        vector(size_t size);
        vector(std::initializer_list<T> elements);

        
        
    private:
        size_t size_{1};
        T array_[];
    };
}

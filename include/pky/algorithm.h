#pragma once

#include <iostream>
#include <typeinfo>

namespace pky
{
    template<typename InIter, typename OutIter>
    OutIter copy(InIter first, InIter last, OutIter out)
    {
        while(first != last)
        {
            *out = *first;
            ++first;
            ++out;
        }

        return out;
    }
}

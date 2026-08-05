#pragma once

namespace pky
{
    template<typename T>
    class shared_ptr
    {
    public:
        shared_ptr() = default;

    private:
        T* ptr_;
        size_t& ref_count;
    };
}
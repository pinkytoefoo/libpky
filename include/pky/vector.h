#pragma once

#include <iostream>
#include <initializer_list>
#include <pky/algorithm.h>
#include <cstring>

namespace pky
{
    template<typename T>
    struct default_allocator
    {
        T* alloc(size_t count)
        {
            return static_cast<T*>(::operator new(count * sizeof(T)));
        }

        void construct_at(T* ptr, size_t pos, const T& obj)
        {
            ::new (static_cast<void*>(ptr + pos)) T(obj);
        }

        template<typename... Args>
        void construct_at(T* ptr, size_t pos, Args&&... args)
        {
            ::new (static_cast<void*>(ptr + pos)) T(std::forward<Args&&>(args)...);
        }

        void dealloc(T* ptr)
        {
            ::operator delete(ptr);
        }
    };

    template<typename T, class Allocator = default_allocator<T>>
    class vector
    {
    public:
        vector() = default;
        vector(size_t cap)
            : capacity_{cap}
            , size_{0}
            , elements_(cap != 0 ? allocator.alloc(cap) : nullptr)
        {
        }

        vector(size_t cap, const T& val)
            : capacity_{cap}
            , size_{cap}
            , elements_{cap != 0 ? allocator.alloc(cap) : nullptr}
        {
            for(size_t i{}; i < capacity_; ++i)
            {
                ::new (static_cast<void*>(elements_ + i)) T(val);
            }
        }

        vector(std::initializer_list<T> elements)
        {
            capacity_ = elements.size();
            size_ = elements.size();
            elements_ = capacity_ != 0 ? static_cast<T*>(::operator new(capacity_ * sizeof(T))) : nullptr;
            pky::copy(elements.begin(), elements.end(), elements_);
        }

        vector(const vector& other)
            : capacity_{other.capacity_}
            , size_{other.size_}
            , elements_{other.capacity_ != 0 ? static_cast<T*>(::operator new(other.capacity_ * sizeof(T))) : nullptr}
        {
            std::cout << "copy constructor!\n";
            T* other_temp = other.elements_;
            T* elem_temp = elements_;

            for(size_t i{}; i < size_; ++i)
            {
                allocator.construct_at(elements_, i, other.elements_[i]);
            }
        }

        vector& operator=(const vector& other)
        {
            if(this != &other)
            {
                capacity_ = other.capacity_;
                size_ = other.size_;
                std::memcpy(elements_, other.elements_, capacity_);
            }

            return *this;
        }

        ~vector()
        {
            clear();
            allocator.dealloc(elements_);
        }


        void push_back(const T& element)
        {
            should_grow_();
        
            allocator.construct_at(elements_, size_, element);
            ++size_;
        }

        void push_back(T&& element)
        {
            should_grow_();
            
            allocator.construct_at(elements_, size_, std::move(element));
            ++size_;
        }
        
        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            should_grow_();
            
            allocator.construct_at(elements_, size_, std::forward<Args&&>(args)...);
            ++size_;
        }

        void reserve(size_t new_cap)
        {
            capacity_ = new_cap;
            elements_ = allocator.alloc(capacity_);
        }
        
        // TODO: implement
        // void resize(size_t new_size)
        // {
        //
        // }

        void clear()
        {
            while(size_ > 0)
            {
                pop_back();
            }
        }

        void pop_back()
        {
            // is decrementing correct here?
            elements_[size_--].~T();
        }

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }

        T& operator[](size_t idx) { return elements_[idx]; }
        const T& operator[](size_t idx) const { return elements_[idx]; }
    private:
        void should_grow_()
        {
            if(size_ < capacity_)
                return;
            
            capacity_ = capacity_ != 0 ? capacity_ * 2 : 1;
            T* temp = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
            for(size_t i{}; i < size_; ++i)
              ::new (static_cast<void*>(temp + i)) T(std::move(elements_[i]));
            
            ::operator delete(elements_);
            elements_ = temp;
        }

        size_t capacity_{0};
        size_t size_{0};
        T* elements_{nullptr};
        [[no_unique_address]] Allocator allocator{};
    };
}

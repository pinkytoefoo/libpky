#pragma once

#include <iostream>
#include <initializer_list>
#include <pky/algorithm.h>
#include <cstring>

namespace pky
{
    template<typename T>
    class vector
    {
    public:
        vector() = default;
        vector(size_t cap)
            : capacity_{cap}, size_{0}, elements_(cap != 0 ? ::operator new(cap * sizeof(T)) : nullptr)
        {
        }

        vector(size_t cap, const T& val)
            : capacity_{cap}
            , size_{cap}
            , elements_{static_cast<T*>(::operator new(cap * sizeof(T)))}
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
                ::new (static_cast<void*>(elements_ + i)) T(other.elements_[i]);
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
            ::operator delete(elements_);
        }


        void push_back(const T& element)
        {
            should_grow_();
        
            ::new (static_cast<void*>(elements_ + size_)) T(element);
            ++size_;
        }

        void push_back(T&& element)
        {
            should_grow_();
        
            ::new (static_cast<void*>(elements_ + size_)) T(std::move(element));
            ++size_;
        }
        
        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            should_grow_();

            elements_[size++] = vector(args...);
        }

        void clear()
        {
            while(size_ > 0)
            {
                pop_back();
            }
        }

        void pop_back()
        {
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
    };
}

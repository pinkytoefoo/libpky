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
            : capacity_{cap}, size_{0}, elements_(cap != 0 ? new T[cap] : nullptr)
        {
        }

        vector(std::initializer_list<T> elements)
        {
            capacity_ = elements.size();
            size_ = elements.size();
            elements_ = capacity_ != 0 ? new T[capacity_] : nullptr;
            pky::copy(elements.begin(), elements.end(), elements_);
        }

        vector(const vector& other)
            : capacity_{other.capacity_}
            , size_{other.size_}
            , elements_{other.capacity_ != 0 ? elements_ = new T[other.capacity_] : elements_ = nullptr}
        {
            std::cout << "copy constructor!\n";
            std::memcpy(elements_, other.elements_, capacity_);
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
            delete[] elements_;
        }


        void push_back(const T& element)
        {
            should_grow_();
        
            elements_[size_++] = element;
        }

        void push_back(T&& element)
        {
            should_grow_();
        
            elements_[size_++] = std::move(element);
        }
        
        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            should_grow_();

            elements_[size++] = vector(args...);
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
            T* temp = ::operator new(capacity_ * sizeof(T));
            for(size_t i{}; i < size_; ++i)
              temp[i] = std::move(elements_[i]);
            
            delete[] elements_;
            elements_ = temp;
        }

        size_t capacity_{0};
        size_t size_{0};
        T* elements_{nullptr};
    };
}

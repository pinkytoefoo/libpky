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
        [[nodiscard]] T* alloc(size_t count)
        {
            return static_cast<T*>(::operator new(count * sizeof(T)));
        }
        
        template<typename... Args>
        void construct_at(T* ptr, Args&&... args)
        {
            ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
        }
        
        void destroy_at(T* ptr)
        {
            ptr->~T();
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
            , elements_(cap != 0 ? allocator_.alloc(cap) : nullptr)
        {
        }

        vector(size_t cap, const T& val)
            : capacity_{cap}
            , size_{cap}
            , elements_{cap != 0 ? allocator_.alloc(cap) : nullptr}
        {
            for(size_t i{}; i < capacity_; ++i)
            {
                allocator_.construct_at(elements_ + i, val);
            }
        }

        vector(std::initializer_list<T> elements)
        {
            capacity_ = elements.size();
            size_ = elements.size();
            elements_ = capacity_ != 0 ? allocator_.alloc(capacity_) : nullptr;
            size_t i{};
            for(const auto& e : elements)
                allocator_.construct_at(elements_ + i++, e);
        }

        vector(const vector& other)
        {
            capacity_ = other.capacity_;
            size_ = other.size_;
            elements_ = capacity_ != 0 ? allocator_.alloc(other.capacity_) : nullptr;
            
            for(size_t i{}; i < other.size_; ++i)
                allocator_.construct_at(elements_ + i, other.elements_[i]);
        }

        vector& operator=(const vector& rhs)
        {
            if(this == &rhs)
                return *this;

            T* temp = rhs.capacity_ != 0 ? allocator_.alloc(rhs.capacity_) : nullptr;
            for(size_t i{}; i < rhs.size_; ++i)
                allocator_.construct_at(temp + i, rhs.elements_[i]);

            clear();
            allocator_.dealloc(elements_);
            elements_ = temp;
            capacity_ = rhs.capacity_;
            size_ = rhs.size_;
            return *this;
        }

        vector(vector&& other)
            : capacity_{other.capacity_}
            , size_{other.size_}
            , elements_{other.elements_}
            , allocator_{std::move(other.allocator_)}
        {
            other.capacity_ = 0;
            other.size_ = 0;
            other.elements_ = nullptr;
        }

        vector& operator=(vector&& rhs)
        {
            if(this == &rhs)
                return *this;
            
            clear();
            allocator_.dealloc(elements_);
            capacity_ = rhs.capacity_;
            size_ = rhs.size_;
            elements_ = rhs.elements_;
            allocator_ = rhs.allocator_;

            rhs.capacity_ = 0;
            rhs.size_ = 0;
            rhs.elements_ = nullptr;

            return *this;
        }

        // void swap(vector& other)
        // {
        //     ::swap(*this, other);
        // }

        friend void swap(vector& first, vector& second) noexcept
        {
            using std::swap;
            swap(first.capacity_, second.capacity_);
            swap(first.size_, second.size_);
            swap(first.elements_, second.elements_);
            swap(first.allocator_, second.allocator_); // not sure if this might be an issue with [[no_unique_address]]
        }

        ~vector() noexcept
        {
            clear();
            allocator_.dealloc(elements_);
        }

        void push_back(const T& element)
        {
            grow_if_needed_();

            allocator_.construct_at(elements_ + size_, element);
            ++size_;
        }

        void push_back(T&& element)
        {
            grow_if_needed_();

            allocator_.construct_at(elements_ + size_, std::move(element));
            ++size_;
        }

        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            grow_if_needed_();
            
            allocator_.construct_at(elements_ + size_, std::forward<Args>(args)...);
            ++size_;
        }

        void reserve(size_t new_cap)
        {
            if(new_cap <= capacity_)
                return;

            reallocate_(new_cap);
        }
        
        // TODO: implement
        // void resize(size_t new_size)
        // {
        //
        // }

        void clear()
        {
            for (size_t i = 0; i < size_; ++i)
                allocator_.destroy_at(elements_ + i);
            
            size_ = 0;
        }

        void pop_back()
        {
            if(size_ == 0)
                return;

            --size_;
            allocator_.destroy_at(elements_ + size_);
        }

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }

        T& operator[](size_t idx) { return elements_[idx]; }
        const T& operator[](size_t idx) const { return elements_[idx]; }
    
    private:
        void grow_if_needed_()
        {
            if(size_ >= capacity_)
                reallocate_(capacity_ != 0 ? capacity_ * 2 : 1);
        }
        
        void reallocate_(size_t new_size)
        {
            T* temp = allocator_.alloc(new_size);
            for(size_t i{}; i < size_; ++i)
            {
                allocator_.construct_at(temp + i, std::move(elements_[i]));
                allocator_.destroy_at(elements_ + i);
            }

            for(size_t i{size_}; i < new_size; ++i)
                allocator_.construct_at(temp + i, T{});
            
            allocator_.dealloc(elements_);
            elements_ = temp;
            capacity_ = new_size;
        }

        size_t capacity_{0};
        size_t size_{0};
        T* elements_{nullptr};
        [[no_unique_address]] Allocator allocator_{};
    };
}

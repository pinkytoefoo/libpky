#pragma once

#include <iostream>
#include <initializer_list>
#include <pky/algorithm.h>
#include <cstring>
#include <stdexcept>

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

    template<class vec>
    class vector_iterator
    {
    public:
        using value_type = typename vec::value_type;
        using pointer = value_type*;
        using reference = value_type&;
    
    public:
        vector_iterator(pointer ptr)
            : ptr_{ptr}
        {
        }
        
        vector_iterator& operator++() { ++ptr_; return *this; }
        vector_iterator operator++(int)
        {
            vector_iterator iter = *this;
            ++ptr_;
            return iter;
        }
        vector_iterator& operator--() { --ptr_; return *this; }
        vector_iterator operator--(int)
        {
            vector_iterator iter = *this;
            --ptr_;
            return iter;
        }
        reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }

        bool operator==(const vector_iterator& other) { return ptr_ == other.ptr_; }
        bool operator!=(const vector_iterator& other) { return !(ptr_ == other.ptr_); }

        reference operator[](size_t idx) { return ptr_[idx]; }

    private:
        pointer ptr_;
    };

    template<typename T, class Allocator = default_allocator<T>>
    class vector
    {
    public:
        using value_type = T;
        using iterator = vector_iterator<vector<T>>;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
    
    public:
        vector() = default;
        vector(size_t cap)
            : capacity_{cap}
            , size_{cap}
            , elements_(cap != 0 ? allocator_.alloc(cap) : nullptr)
        {
            // default construct elements, same as stl vector
            for(size_t i{}; i < cap; ++i)
                allocator_.construct_at(elements_ + i, T());
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

            vector temp(rhs);
            swap(temp);
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
            
            vector temp(std::move(rhs));
            swap(temp);

            return *this;
        }
        
        void swap(vector& other)
        {
            using std::swap;
            swap(*this, other);
        }

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
            append_back_(element);
        }

        void push_back(T&& element)
        {
            append_back_(std::move(element));
        }

        template<typename... Args>
        void emplace_back(Args&&... args)
        {
            append_back_(std::forward<Args>(args)...);
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

        pointer data() noexcept { return elements_; }
        const_pointer data() const noexcept { return elements_; }

        // void shrink_to_fit()
        // {
        //     if(capacity_ > size_)
        //         allocator_.dealloc(elements_ + size_);
        // }


        iterator begin() { return iterator(elements_); }
        iterator end() { return iterator(elements_ + size_); }
        

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }

        T& at(size_t idx)
        {
            if(idx >= size_)
                throw std::out_of_range(std::format("pky::vector<>.at(size_t idx) - idx({}) >= size({})", idx, size_));
            
            return elements_[idx];
        }

        const T& at(size_t idx) const
        {
            if(idx >= size_)
                throw std::out_of_range(std::format("pky::vector<>.at(size_t idx) - idx({}) >= size({})", idx, size_));
            
            return elements_[idx];
        }

        reference operator[](size_t idx) { return elements_[idx]; }
        const_reference operator[](size_t idx) const { return elements_[idx]; }

        reference front() { return elements_[0]; }
        const_reference front() const { return elements_[0]; }
        reference back() { return elements_[size_ - 1]; }
        const_reference back() const { return elements_[size_ - 1]; }
    
    private:
        void grow_if_needed_()
        {
            if(size_ >= capacity_)
                reallocate_(capacity_ != 0 ? capacity_ * 2 : 1);
        }
        
        void reallocate_(size_t new_size)
        {
            T* temp = allocator_.alloc(new_size);

            // acts as temp size
            size_t i{};
            try
            {
                for(; i < size_; ++i)
                    allocator_.construct_at(temp + i, std::move_if_noexcept(elements_[i]));
            }
            catch(...)
            {
                for (size_t j{0}; j < i; ++j)
                    allocator_.destroy_at(elements_ + j);
                
                allocator_.dealloc(temp);
                throw;
            }

            for (; i < size_; ++i)
                allocator_.destroy_at(elements_ + i);
            
            allocator_.dealloc(elements_);

            elements_ = temp;
            capacity_ = new_size;
        }

        template<typename... Args>
        void append_back_(Args&&... args)
        {
            grow_if_needed_();

            allocator_.construct_at(elements_ + size_, std::forward<Args>(args)...);
            ++size_;
        }

        size_t capacity_{0};
        size_t size_{0};
        pointer elements_{nullptr};
        [[no_unique_address]] Allocator allocator_{};
    };
}

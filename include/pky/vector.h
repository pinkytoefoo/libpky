#pragma once

#include <iostream>
#include <initializer_list>
#include <pky/algorithm.h>
#include <cstring>
#include <stdexcept>
#include <format>

namespace pky
{
    template<typename T>
    struct default_allocator
    {
        using value_type = T;
        using pointer = T*;

        [[nodiscard]] pointer allocate(size_t count)
        {
            return static_cast<pointer>(::operator new(count * sizeof(T)));
        }
        
        template<typename... Args>
        void construct_at(pointer ptr, Args&&... args)
        {
            ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
        }
        
        void destroy_at(pointer ptr)
        {
            ptr->~T();
        }

        void deallocate(pointer ptr)
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
            , elements_(cap != 0 ? allocator_.allocate(cap) : nullptr)
        {
            construct_all_();
        }

        vector(size_t cap, const T& val)
            : capacity_{cap}
            , size_{cap}
            , elements_{cap != 0 ? allocator_.allocate(cap) : nullptr}
        {
            construct_all_(val);
        }

        vector(std::initializer_list<T> elements)
            : capacity_{elements.size()}
            , size_{elements.size()}
            , elements_{capacity_ != 0 ? allocator_.allocate(capacity_) : nullptr}
        {
            size_t i{};
            for(const auto& e : elements)
                allocator_.construct_at(elements_ + i++, e);
        }

        vector(const vector& other)
            : capacity_{other.capacity_}
            , size_{other.size_}
            , elements_{capacity_ != 0 ? allocator_.allocate(other.capacity_) : nullptr}
        {
            std::cout << "copy ctor\n";
            
            construct_from_(other);
        }

        vector& operator=(const vector& rhs)
        {
            std::cout << "copy assign\n";
            if(this == &rhs)
                return *this;

            vector temp(rhs);
            swap(temp);
            
            return *this;
        }

        vector(vector&& other) noexcept
            : capacity_{other.capacity_}
            , size_{other.size_}
            , elements_{other.elements_}
            , allocator_{std::move(other.allocator_)}
        {
            std::cout << "move ctor\n";

            other.default_();
        }

        vector& operator=(vector&& rhs) noexcept
        {
            std::cout << "move assign noexcept\n";

            if(this == &rhs)
                return *this;
            
            destroy_and_deallocate_();
            capacity_ = rhs.capacity_;
            size_ = rhs.size_;
            elements_ = rhs.elements_;
            allocator_ = std::move(rhs.allocator_);

            rhs.default_();

            return *this;
        }
        
        void swap(vector& other) noexcept
        {
            using std::swap;
            swap(capacity_, other.capacity_);
            swap(size_, other.size_);
            swap(elements_, other.elements_);
            swap(allocator_, other.allocator_);
        }

        friend void swap(vector& first, vector& second) noexcept
        {
            using std::swap;
            first.swap(second);
        }

        ~vector() noexcept
        {
            destroy_and_deallocate_();
        }

        template<std::convertible_to<T> U>
        void push_back(U&& element)
        {
            append_back_(std::forward<U>(element));
        }

        template<typename... Args>
        reference emplace_back(Args&&... args)
        {
            append_back_(std::forward<Args>(args)...);
            return back();
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

        void shrink_to_fit()
        {
            if(capacity_ <= size_)
                return;
            
            destroy_and_deallocate_();
        }


        iterator begin() { return iterator(elements_); }
        iterator end() { return iterator(elements_ + size_); }
        

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }

        reference at(size_t idx)
        {
            if(idx >= size_)
                throw std::out_of_range(std::format("pky::vector<>.at(size_t idx) - idx({}) >= size({})", idx, size_));
            
            return elements_[idx];
        }

        const_reference at(size_t idx) const
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
        size_t capacity_{0};
        size_t size_{0};
        pointer elements_{nullptr};
        [[no_unique_address]] Allocator allocator_{};
    
    private:
        // HELPERS
        void destroy_and_deallocate_()
        {
            clear();
            allocator_.deallocate(elements_);
        }

        void grow_if_needed_()
        {
            if(size_ >= capacity_)
                reallocate_(capacity_ != 0 ? capacity_ * 2 : 1);
        }
        
        void reallocate_(size_t new_size)
        {
            T* temp = allocator_.allocate(new_size);

            size_t i{};
            try
            {
                for(; i < size_; ++i)
                    allocator_.construct_at(temp + i, std::move_if_noexcept(elements_[i]));
            }
            catch(...)
            {
                for (size_t j{0}; j < i; ++j)
                    allocator_.destroy_at(temp + j);
                
                allocator_.deallocate(temp);
                throw;
            }

            for (i = 0; i < size_; ++i)
                allocator_.destroy_at(elements_ + i);
            
            allocator_.deallocate(elements_);

            elements_ = temp;
            capacity_ = new_size;
        }

        template<typename... Args>
        void construct_all_(Args&&... args)
        {
            size_t i{};
            try
            {
                for(; i < size_; ++i)
                    allocator_.construct_at(elements_ + i, std::forward<Args>(args)...);
            }
            catch (...)
            {
                for(size_t j{}; j < i; ++j)
                    allocator_.destroy_at(elements_ + j);
                
                allocator_.deallocate(elements_);
                throw;
            }
        }

        void construct_from_(const vector& other)
        {
            size_t i{};
            try
            {
                for(; i < size_; ++i)
                    allocator_.construct_at(elements_ + i, other[i]); // possible size indexing issue, idk
            }
            catch (...)
            {
                for(size_t j{}; j < i; ++j)
                    allocator_.destroy_at(elements_ + j);
                
                allocator_.deallocate(elements_);
                throw;
            }
        }

        // TODO: find a better name
        /**
         * @brief sets all values to default
         * @important call this after clearing and deallocating, since this function sets elements_ to nullptr, possibly causing leaks if not used right
         */
        void default_()
        {
            capacity_ = 0;
            size_ = 0;
            elements_ = nullptr;
        }

        // template<typename Iter, typename... Args>
        // void construct_from_iter_(Iter iter, Args&&... args)
        // {
        //     size_t i{};
        //     for(const auto& thing : iter)
        //         allocator_.construct_at(elements_ + i++, thing);
        // }

        template<typename... Args>
        void append_back_(Args&&... args)
        {
            grow_if_needed_();

            allocator_.construct_at(elements_ + size_, std::forward<Args>(args)...);
            ++size_;
        }
    };
}

#pragma once

#include <initializer_list>
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
            std::copy(elements.begin(), elements.end(), elements_);
        }

        vector(const vector& other)
            : capacity_{other.capacity_}
            , size_{other.size_}
            , elements_{other.capacity_ != 0 ? elements_ = new T[other.capacity_] : elements_ = nullptr}
        {
            std::copy(other.elements_.begin(), other.elements_.end(), elements_);
        }

        ~vector()
        {
            delete[] elements_;
        }


        void push_back(const T& element)
        {
            if(size_ >= capacity_)
                grow_();
        
            elements_[size_++] = element;
        }

        void push_back(T&& element)
        {
            if(size_ >= capacity_)
                grow_();
        
            elements_[size_++] = std::move(element);
        }

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }

        T& operator[](size_t idx) { return elements_[idx]; }
    private:
        void grow_()
        {
            capacity_ = capacity_ != 0 ? capacity_ * 2 : 1;
            T* temp = new T[capacity_];
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

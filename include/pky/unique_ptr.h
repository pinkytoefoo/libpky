#pragma once


#include <utility>
#include <cstddef>

namespace pky
{
    template<typename T>
    struct default_delete
    {
        void operator()(T* ptr) const
        {
            delete ptr;
        }
    };

    template<typename T, class Deleter = default_delete<T>>
    class unique_ptr
    {
    public:
        unique_ptr() = default;
        explicit unique_ptr(T* ptr)
            : ptr_{ptr}
        {
        }

        unique_ptr(T* ptr, const Deleter& custom_deleter)
            : ptr_{ptr}
            , deleter_{custom_deleter}
        {
        }

        unique_ptr(T* ptr, Deleter&& custom_deleter)
            : ptr_{ptr}
            , deleter_{std::move(custom_deleter)}
        {
        }

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        unique_ptr(unique_ptr&& other)
            : ptr_{other.release()}
            , deleter_{std::move(other.deleter_)}
        {
        }

        unique_ptr& operator=(unique_ptr&& other)
        {
            if(this != &other)
            {
                reset(other.release());
                deleter_ = std::move(other.deleter_);
            }
            return *this;
        }

        unique_ptr(std::nullptr_t)
        {
        }

        unique_ptr& operator=(std::nullptr_t)
        {
            reset();
            return *this;
        }

        ~unique_ptr()
        {
            reset();
        }

        [[nodiscard]] T* release()
        {
            T* temp = ptr_;
            ptr_ = nullptr;
            return temp;
        }

        void reset(T* new_ptr = nullptr)
        {
            T* old_ptr = std::exchange(ptr_, new_ptr);

            if(old_ptr)
                deleter_(old_ptr);
        }

        void swap(unique_ptr& other)
        {
            using std::swap;
            swap(ptr_, other.ptr_);
            swap(deleter_, other.deleter_);
        }

        [[nodiscard]] T* get() const { return ptr_; }
        [[nodiscard]] Deleter& get_deleter() { return deleter_; }
        [[nodiscard]] const Deleter& get_deleter() const { return deleter_; }

        [[nodiscard]] explicit operator bool() const { return ptr_ != nullptr; }
        T& operator*() const { return *ptr_; }
        T* operator->() const { return ptr_; }
    private:
    // #ifdef __cplusplus >= 202302L
        T* ptr_{nullptr};
        [[no_unique_address]] Deleter deleter_{};
    // #else
    //     // TODO: this
    //     struct members : public Deleter
    //     {
    //         members(T* ptr)
    //             : ptr{ptr}
    //             , Deleter{}
    //         {
    //         }
    //         T* ptr_{nullptr};
    //     };

    //     members m_;
    // #endif
    };

    template<typename T>
    bool operator==(const unique_ptr<T>& a, const unique_ptr<T>& b) { return a.get() == b.get(); }

    template<typename T>
    bool operator!=(const unique_ptr<T>& a, const unique_ptr<T>& b) { return !(a.get() == b.get()); }

    template<typename T>
    bool operator==(const unique_ptr<T>& a, std::nullptr_t) { return !a.get(); }

    template<typename T>
    bool operator!=(const unique_ptr<T>& a, std::nullptr_t) { return a.get(); }

    template<typename T, typename Deleter = default_delete<T>, typename... Args>
    unique_ptr<T, Deleter> make_unique(Args&&... args)
    {
        return unique_ptr<T, Deleter>(new T(std::forward<Args>(args)...));
    }
}

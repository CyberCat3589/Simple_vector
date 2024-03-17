#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

template <typename Type> class ArrayPtr
{
  public:
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size)
    {
        if (size)
        {
            raw_ptr_ = new Type[size]();
        }
        else
        {
            raw_ptr_ = nullptr;
        }
    }

    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr()
    {
        delete[] raw_ptr_;
    }

    ArrayPtr(const ArrayPtr& other, size_t other_size, size_t new_size)
    {
        raw_ptr_ = new Type[new_size];
        std::copy(other.raw_ptr_, other.raw_ptr_ + other_size, raw_ptr_);
    }

    Type& operator[](size_t index) noexcept
    {
        return raw_ptr_[index];
    }

    const Type& operator[](size_t index) const noexcept
    {
        return raw_ptr_[index];
    }

    explicit operator bool() const
    {
        return raw_ptr_;
    }

    void swap(ArrayPtr& other) noexcept
    {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept
    {
        return &raw_ptr_[0];
    }

  private:
    Type* raw_ptr_ = nullptr;
};
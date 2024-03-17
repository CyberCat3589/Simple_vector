#pragma once

#include "array_ptr.h"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

template <typename Type> 
class SimpleVector
{
  public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : simple_vector_(size), size_(size), capacity_(size){}

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : simple_vector_(size), size_(size), capacity_(size)
    {
        std::fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : simple_vector_(init.size()), size_(init.size()), capacity_(init.size())
    {
        std::copy(init.begin(), init.end(), begin());
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept
    {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept
    {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept
    {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept
    {
        return simple_vector_[index];
    }

    const Type& operator[](size_t index) const noexcept
    {
        return simple_vector_[index];
    }

    Type& At(size_t index)
    {
        if (index < size_)
        {
            return simple_vector_[index];
        }
        else
        {
            throw std::out_of_range("Non-existent vector element.");
        }
    }

    const Type& At(size_t index) const
    {
        if (index < size_)
        {
            return simple_vector_[index];
        }
        else
        {
            throw std::out_of_range("Non-existent vector element.");
        }
    }

    void Clear() noexcept
    {
        size_ = 0;
    }

    void Resize(size_t new_size)
    {
        if (size_ >= new_size)
        {
            size_ = new_size;
            return;
        }
        else if (size_ < new_size && capacity_ > new_size)
        {
            std::fill(end(), begin() + new_size, Type());
            size_ = new_size;
            return;
        }
        else
        {
            ArrayPtr<Type> helper(new_size);
            std::copy(begin(), end(), &helper[0]);
            simple_vector_.swap(helper);
            size_ = new_size;
            capacity_ = new_size * 2;
        }
    }

    Iterator begin() noexcept
    {
        return Iterator(&simple_vector_[0]);
    }

    Iterator end() noexcept
    {
        return Iterator(&simple_vector_[size_]);
    }

    ConstIterator begin() const noexcept
    {
        return cbegin();
    }

    ConstIterator end() const noexcept
    {
        return cend();
    }

    ConstIterator cbegin() const noexcept
    {
        return ConstIterator(&simple_vector_[0]);
    }

    ConstIterator cend() const noexcept
    {
        return ConstIterator(&simple_vector_[size_]);
    }

  private:
    ArrayPtr<Type> simple_vector_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};
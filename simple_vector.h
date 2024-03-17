#pragma once

#include "array_ptr.h"
#include <algorithm>
#include <initializer_list>
#include <stdexcept>

// Не забудьте обновить указатель на начало вектора и текущую ёмкость.Для
// реализации конструктора с резервированием вам понадобится
// дополнительный класс - обёртка, чтобы компилятор мог разобраться и вызвать
// правильный конструктор.Этот конструктор должен принимать по значению
// объект этого класса - обёртки.Тогда функция(не метод!) будет иметь следующую сигнатуру :
class ReserveProxyObj
{
  public:
    explicit ReserveProxyObj(size_t capacity_to_reserve) : capacity_(capacity_to_reserve)
    {
    }

    size_t ReserveCapacity()
    {
        return capacity_;
    }

  private:
    size_t capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve)
{
    return ReserveProxyObj(capacity_to_reserve);
};

template <typename Type> class SimpleVector
{
  public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : items_(size), size_(size), capacity_(size)
    {
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : items_(size), size_(size), capacity_(size)
    {
        std::fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : items_(init.size()), size_(init.size()), capacity_(init.size())
    {
        std::copy(init.begin(), init.end(), begin());
    }

    explicit SimpleVector(ReserveProxyObj obj)
    {
        Reserve(obj.ReserveCapacity());
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept
    {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept
    {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept
    {
        return !size_;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept
    {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept
    {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index)
    {
        if (index < size_)
        {
            return items_[index];
        }
        else
        {
            throw std::out_of_range("Non-existent vector element.");
        }
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const
    {
        if (index < size_)
        {
            return items_[index];
        }
        else
        {
            throw std::out_of_range("Non-existent vector element.");
        }
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept
    {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
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
            items_.swap(helper);
            size_ = new_size;
            capacity_ = new_size * 2;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept
    {
        return Iterator(&items_[0]);
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept
    {
        return Iterator(&items_[size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept
    {
        return cbegin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept
    {
        return cend();
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept
    {
        return ConstIterator(&items_[0]);
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept
    {
        return ConstIterator(&items_[size_]);
    }

    SimpleVector(const SimpleVector& other)
    {
        ArrayPtr<Type> helper(other.size_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        std::copy(other.begin(), other.end(), &helper[0]);
        items_.swap(helper);
    }

    SimpleVector& operator=(const SimpleVector& rhs)
    {
        if (this != &rhs)
        {
            SimpleVector helper(rhs);
            swap(helper);
        }
        return *this;
    }

    void PushBack(const Type& item)
    {
        this->Resize(size_ + 1);
        items_[size_ - 1] = item;
    }

    Iterator Insert(ConstIterator pos, const Type& value)
    {
        size_t quantity = pos - items_.Get();
        if (capacity_ == 0)
        {
            ArrayPtr<Type> tmp(1);
            tmp[quantity] = value;
            items_.swap(tmp);
            ++capacity_;
            ++size_;
        }
        else if (size_ < capacity_)
        {
            std::copy_backward(items_.Get() + quantity, items_.Get() + size_, items_.Get() + size_ + 1);
            items_[quantity] = value;
            ++size_;
        }
        else
        {
            size_t tmp_c = std::max(size_ + 1, capacity_ * 2);
            ArrayPtr<Type> tmp(tmp_c);
            std::copy(items_.Get(), items_.Get() + size_, tmp.Get());
            std::copy_backward(items_.Get() + quantity, items_.Get() + size_, tmp.Get() + size_ + 1);
            tmp[quantity] = value;
            items_.swap(tmp);
            capacity_ = tmp_c;
            ++size_;
        }
        return &items_[quantity];
    }

    void PopBack() noexcept
    {
        if (size_)
            --size_;
    }

    Iterator Erase(ConstIterator pos)
    {
        auto index = std::distance(cbegin(), pos);
        std::copy(++pos, cend(), &items_[index]);
        --size_;
        return Iterator(&items_[index]);
    }

    void swap(SimpleVector& other) noexcept
    {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void Reserve(size_t new_capacity)
    {
        if (new_capacity > capacity_)
        {
            ArrayPtr<Type> temp(new_capacity);
            std::fill(temp.Get(), temp.Get() + new_capacity, Type());
            std::copy(items_.Get(), items_.Get() + size_, temp.Get());
            items_.swap(temp);
            capacity_ = new_capacity;
        }
    }

  private:
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <typename Type> inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type> inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return !(lhs == rhs);
}

template <typename Type> inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type> inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return lhs < rhs || lhs == rhs;
}

template <typename Type> inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return rhs < lhs;
}

template <typename Type> inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return !(lhs < rhs);
}
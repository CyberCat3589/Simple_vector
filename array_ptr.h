#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

using namespace std;

template <typename Type> 
class ArrayPtr
{
  public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле simple_vector_ должно быть равно nullptr
    explicit ArrayPtr(size_t size)
    {
        if (size)
        {
            simple_vector_ = new Type[size]();  // добавили скобки ()
        }
        else
        {
            simple_vector_ = nullptr;
        }
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept
    {
        simple_vector_ = raw_ptr;
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr()
    {
        // Напишите деструктор самостоятельно
        delete[] simple_vector_;
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept
    {
        Type* releasedPtr = simple_vector_;
        simple_vector_ = nullptr;
        return releasedPtr;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept
    {
        return simple_vector_[index];
    }
    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept
    {
        return simple_vector_[index];
    }
    explicit operator bool() const
    {
        return simple_vector_;
    }
    
    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
      if (simple_vector_ ) {
      return true;
      }
        return false;
    }
   
    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept
    {
        return simple_vector_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept
    {
        std::swap(simple_vector_, other.simple_vector_);
    }

  private:
    // size_t size;   если менять на другие варианты
    Type* simple_vector_ = nullptr;
};
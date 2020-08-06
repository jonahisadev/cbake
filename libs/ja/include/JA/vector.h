#pragma once

#include "JA.h"
#include "iterator.h"

#include <exception>

class VectorException : public std::exception
{
private:
    const char* _what;

public:
    VectorException(const char* what) : _what(what) {}

    const char* what() const throw()
    {
        return _what;
    }
};

template <typename T>
class Vector
    : public Iterable<T>
{
private:
    T* _data;
    int _ptr;
    int _size;

public:
    Vector(int size = 1);
    Vector(const Vector& other);
    ~Vector();

    Vector<T>& operator=(const Vector<T>& other);

    void push_back(const T& data);

    template <typename... Args>
    void emplace_back(Args... args)
    {
        T t(args...);
        push_back(t);
    }

    inline int size() const { return _ptr; };
    const T& at(int index) const;

    const T& operator[](int index) const;
    Vector<T>& operator<<(const T& data);

    JA_STL_ITERATOR(T);
    stl_iterator begin() { return stl_iterator(_data); }
    stl_iterator end() { return stl_iterator(_data + _ptr); }
};

template <typename T>
Vector<T>::Vector(int size)
{
    _data = ALLOC(T, size);
    _ptr = 0;
    _size = size;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other)
{
    for (int i = 0; i < _ptr; i++)
        _data[i] = other._data[i];
    _size = other._size;
    _ptr = other._ptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
    if (&other != this) {
        for (int i = 0; i < other._ptr; i++)
            _data[i] = other._data[i];
        _ptr = other._ptr;
        _size = other._size;
    }

    return *this;
}

template <typename T>
Vector<T>::~Vector()
{
    FREE(T, _data);
}

template <typename T>
void Vector<T>::push_back(const T& data)
{
    while (_ptr >= _size) {
        // Start realloc
        T* temp = _data;
        _data = ALLOC(T, _size * 2);
        
        // Copy old data to new location
        for (int i = 0; i < _size; i++)
            _data[i] = temp[i];

        // New size
        _size *= 2;

        // Free old data
        FREE(T, temp);
    }

    _data[_ptr++] = data;
}

template <typename T>
const T& Vector<T>::at(int index) const
{
    if (index < 0 || index > _ptr)
        throw VectorException("Out of bounds");
    return _data[index];
}

template <typename T>
const T& Vector<T>::operator[](int index) const
{
    return at(index);
}

template <typename T>
Vector<T>& Vector<T>::operator<<(const T& data)
{
    push_back(data);
    return *this;
}
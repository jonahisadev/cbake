#pragma once

#include "iterator.h"

#include <exception>

class ArrayException : public std::exception
{
private:
    const char* _what;

public:
    ArrayException(const char* what) : _what(what) {}

    const char* what() const throw()
    {
        return _what;
    }
};

template <typename T, unsigned int N>
class Array
    : public Iterable<T>
{
private:
    T _data[N];

public:
    Array() {}
    Array(const Array<T,N>& other);

    template <typename... Args>
    constexpr Array(Args&&... args) : _data{args...} {}

    Array<T,N>& operator=(const Array<T,N>& other);

    T& at(int index);
    T& operator[](int index);

    const T& at(int index) const override;
    int size() const override;

    JA_STL_ITERATOR(T);
    stl_iterator begin() { return stl_iterator(_data); }
    stl_iterator end() { return stl_iterator(_data + N); }
};

template <typename T, unsigned int N>
Array<T,N>::Array(const Array<T,N>& other)
{
    for (unsigned int i = 0; i < N; i++)
        other._data[i] = _data[i];
}

template <typename T, unsigned int N>
Array<T,N>& Array<T,N>::operator=(const Array<T,N>& other)
{
    if (&other == this)
        return *this;

    for (unsigned int i = 0; i < N; i++)
        other._data[i] = _data[i];
}

template <typename T, unsigned int N>
T& Array<T, N>::at(int index)
{
    if (index < 0 || index >= N)
        throw ArrayException("Out of bounds");
    return _data[index];
}

template <typename T, unsigned int N>
T& Array<T, N>::operator[](int index)
{
    return at(index);
}

template <typename T, unsigned int N>
const T& Array<T, N>::at(int index) const
{
    return const_cast<const T&>(at(index));
}

template <typename T, unsigned int N>
int Array<T, N>::size() const
{
    return N;
}
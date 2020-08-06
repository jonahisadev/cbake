#pragma once

template <typename T>
class Iterator;

template <typename T>
class ConstIterator;

template<typename T>
class Iterable
{
public:
    virtual int size() const = 0;
    virtual const T& at(int index) const = 0;

    Iterator<T> iterator() const { return Iterator<T>(*this); }
    ConstIterator<T> constIterator() const { return ConstIterator<T>(*this); }
};

template <typename T>
class IteratorType
{
protected:
    const Iterable<T>& _container;
    int _size;
    int _ptr;

public:
    IteratorType(const Iterable<T>& c);

    bool hasNext();
    T& next();
    T& peekNext() const;

    int index();
};

template <typename T>
class Iterator
    : public IteratorType<T>
{
public:
    Iterator(const Iterable<T>& c) : IteratorType<T>(c) {}
};

template <typename T>
class ConstIterator
    : public IteratorType<T>
{
public:
    ConstIterator(const Iterable<T>& c) : IteratorType<T>(c) {}

    const T& next();
    const T& peekNext() const;
};

template <typename T>
IteratorType<T>::IteratorType(const Iterable<T>& c)
    : _container(c)
{
    _size = c.size();
    _ptr = 0;
}

template <typename T>
bool IteratorType<T>::hasNext()
{
    return (_ptr < _size);
}

template <typename T>
T& IteratorType<T>::next()
{
    return const_cast<T&>(_container.at(_ptr++));
}

template <typename T>
T& IteratorType<T>::peekNext() const
{
    return const_cast<T&>(_container.at(_ptr));
}

template <typename T>
int IteratorType<T>::index()
{
    return _ptr;
}

template <typename T>
const T& ConstIterator<T>::next()
{
    using IT = IteratorType<T>;
    return IT::_container.at(IT::_ptr++);
}

template <typename T>
const T& ConstIterator<T>::peekNext() const
{
    using IT = IteratorType<T>;
    return IT::_container.at(IT::_ptr);
}

template <typename T>
class basic_stl_iterator
{
private:
    T* _data;
public:
    basic_stl_iterator(T* data) : _data(data) {}
    basic_stl_iterator operator++() { ++_data; return *this; }
    bool operator!=(const basic_stl_iterator& other) { return _data != other._data; }
    const T& operator*() const { return *_data; }
};

#define JA_STL_ITERATOR(T) \
    class stl_iterator : public basic_stl_iterator<T> { \
    public: \
        stl_iterator(T* data) : basic_stl_iterator<T>(data) {} \
    }
    
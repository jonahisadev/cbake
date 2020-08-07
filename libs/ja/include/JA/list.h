#pragma once

#include "JA.h"
#include "iterator.h"
#include <exception>

class ListException : public std::exception
{
private:
    const char* _what;

public:
    ListException(const char* what) : _what(what) {}

    const char* what() const throw()
    {
        return _what;
    }
};

// Heap allocated list
template <class T>
class List
    : public Iterable<T>
{
public:
    class stl_iterator
    {
    private:
        T** _data;
        int _ptr;
    public:
        stl_iterator(T** data, int ptr) : _data(data), _ptr(ptr) {}
        stl_iterator operator++() { ++_ptr; return *this; }
        bool operator!=(const stl_iterator& other) const { return _ptr != other._ptr; }
        const T& operator*() const { return *(_data[_ptr]); }
    };

private:
    T** _data;
    int _ptr;
    int _size;

public:
    List(int size = 1);
    List(const List<T>& other);
    ~List();

    List<T>& operator=(const List<T>& other);

    void push_back(const T& t);

    template <class... Args>
    void emplace_back(Args&&... args)
    {
        T t(args...);
        push_back(t);
    }

    int size() const override;
    const T& at(int index) const override;
    const T& last() const;

    void clear();

    T& operator[](int index);
    List<T>& operator<<(const T& data);

    stl_iterator begin() const { return stl_iterator(_data, 0); }
    stl_iterator end() const { return stl_iterator(_data, _ptr); }
};

template <class T>
List<T>::List(int size)
{
    _data = ALLOC(T*, size);
    _ptr = 0;
    _size = size;
}

template <class T>
List<T>::List(const List<T>& other)
{
    _data = ALLOC(T*, other._size);

    for (int i = 0; i < other._ptr; i++) {
        _data[i] = new T(*other._data[i]);
    }

    _ptr = other._ptr;
    _size = other._size;
}

template <class T>
List<T>::~List()
{
    for (int i = 0; i < _ptr; i++)
        delete _data[i];
    FREE(T*, _data);
}

template <class T>
List<T>& List<T>::operator=(const List<T>& other)
{
    if (&other != this) {
        _data = ALLOC(T*, other._size);

        for (int i = 0; i < other._ptr; i++) {
            _data[i] = new T(*other._data[i]);
        }

        _ptr = other._ptr;
        _size = other._size;
    }

    return *this;
}

template <class T>
void List<T>::push_back(const T& t)
{
    T* keep = new T(t);

    while (_ptr >= _size) {
        // Start realloc
        T** temp = _data;
        _data = ALLOC(T*, _size * 2);
        
        // Copy old data to new location
        for (int i = 0; i < _size; i++)
            _data[i] = temp[i];

        // New size
        _size *= 2;

        // Free old data
        FREE(T*, temp);
    }

    _data[_ptr++] = keep;
}

template <class T>
const T& List<T>::at(int index) const
{
    if (index >= _ptr || index < 0)
        throw ListException("Out of bounds");
    return const_cast<const T&>(*_data[index]);
}

template <class T>
int List<T>::size() const
{
    return _ptr;
}

template <class T>
const T& List<T>::last() const
{
    return const_cast<const T&>(*_data[_ptr - 1]);
}

template <class T>
void List<T>::clear()
{
    FREE(T*, _data);
    _data = ALLOC(T*, _size);
}

template <class T>
T& List<T>::operator[](int index)
{
    return const_cast<T&>(at(index));
}

template <class T>
List<T>& List<T>::operator<<(const T& data)
{
    push_back(data);
    return *this;
}

template <typename T>
struct LinkedListNode
{
    T* data;
    LinkedListNode<T>* next = nullptr;
    LinkedListNode<T>* prev = nullptr;
};

template <typename T>
class LinkedList
{
private:
    LinkedListNode<T>* _root;
    int _size;

    LinkedListNode<T>* _it_ptr;
    int _it_index = 0;

public:
    LinkedList();
    ~LinkedList() {}

    void append(const T& data);
    void prepend(const T& data);
    void insert(const T& data);

    bool hasNext();
    T& next();

    void makePrevious();
    bool hasPrevious();
    T& previous();

    void reset();

    int size() const { return _size; }
};

template <typename T>
LinkedList<T>::LinkedList()
{
    _root = nullptr;
    _size = 0;
}

template <typename T>
void LinkedList<T>::append(const T& data)
{
    auto* node = new LinkedListNode<T>();
    node->data = new T(data);

    // First node
    if (!_root) {
        _root = node;
        _it_ptr = _root;
        ++_size;
        return;
    }

    // Single root
    if (!_root->prev) {
        node->next = _root;
        node->prev = _root;
        _root->next = node;
        _root->prev = node;
    }

    // Append
    else {
        auto* to_append = _root->prev;
        to_append->next = node;
        node->prev = to_append;
        _root->prev = node;
    }

    ++_size;
}

template <typename T>
void LinkedList<T>::prepend(const T& data)
{
    auto* node = new LinkedListNode<T>();
    node->data = new T(data);

    // First node
    if (!_root) {
        _root = node;
        _it_ptr = _root;
        ++_size;
        return;
    }

    // Prepend
    auto* copy = _root;
    _root = node;
    node->next = copy;
    node->prev = copy->prev;
    copy->prev = node;
    _it_ptr = _root;
    ++_size;
}

template <typename T>
void LinkedList<T>::insert(const T& data)
{
    if (_it_index == 0) {
        prepend(data);
        return;
    } else if (_it_index == _size) {
        append(data);
        return;
    }

    // Set up data
    auto* node = new LinkedListNode<T>();
    node->data = new T(data);
    auto* before = _it_ptr;

    // Insert node
    node->next = before;
    node->prev = before->prev;
    before->prev->next = node;
    before->prev = node;

    // Update the list
    ++_size;
    ++_it_index;
}

template <typename T>
bool LinkedList<T>::hasNext()
{
    return (_it_index != _size);
}

template <typename T>
T& LinkedList<T>::next()
{
    T& t = *_it_ptr->data;
    _it_ptr = _it_ptr->next;
    ++_it_index;
    return t;
}

template <typename T>
void LinkedList<T>::makePrevious()
{
    _it_ptr = _root->prev;
    _it_index = _size - 1;
}

template <typename T>
bool LinkedList<T>::hasPrevious()
{
    return (_it_index != -1);
}

template <typename T>
T& LinkedList<T>::previous()
{
    T& t = *_it_ptr->data;
    _it_ptr = _it_ptr->prev;
    --_it_index;
    return t;
}

template <typename T>
void LinkedList<T>::reset()
{
    _it_ptr = _root;
    _it_index = 0;
}
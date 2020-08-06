#pragma once

#include <exception>

class PtrException : public std::exception
{
private:
    const char* _what;

public:
    PtrException(const char* what) : _what(what) {}

    const char* what() const throw()
    {
        return _what;
    }
};

template <class T>
class BorrowPtr;

template <class T>
class OwnPtr
{
private:
    T* _ptr;

public:
    OwnPtr()
    {
        _ptr = nullptr;
    }

    OwnPtr(T* ptr)
    {
        _ptr = ptr;
    }

    ~OwnPtr()
    {
        if (_ptr)
            delete _ptr;
    }

    // Move constructor
    OwnPtr(OwnPtr<T>&& other)
    {
        _ptr = other._ptr;
        other._ptr = nullptr;
    }

    // Move assignment
    OwnPtr<T>& operator=(OwnPtr<T>&& other)
    {
        if (other != *this) {
            _ptr = other._ptr;
            other._ptr = nullptr;
        }

        return *this;
    }

    // No copy
    OwnPtr(const OwnPtr<T>& copy) = delete;
    OwnPtr<T>& operator=(const OwnPtr<T>& copy) = delete;

    T& operator*()
    {
        if (_ptr)
            return *_ptr;
        throw PtrException("Dereferrencing OwnPtr<T> by nonpossessor");
    }

    T* operator->()
    {
        if (_ptr)
            return _ptr;
        throw PtrException("Accessing OwnPtr<T> by nonpossessor");
    }

    bool operator==(const OwnPtr<T>& other)
    {
        return _ptr == other._ptr;
    }

    const T* get()
    {
        return _ptr;
    }

    void destroy()
    {
        delete _ptr;
        _ptr = nullptr;
    }

    template <class... Args>
    static inline OwnPtr<T> make(Args&&... args)
    {
        T* t = new T(args...);
        return OwnPtr<T>(t);
    }

    static inline OwnPtr<T> make(T copy)
    {
        T* t = new T(copy);
        return OwnPtr<T>(t);
    }

    friend class BorrowPtr<T>;
};

template <class T, class... Args>
static OwnPtr<T> make_own_ptr(Args&&... args)
{
    return OwnPtr<T>::make(args...);
}

template <class T>
static OwnPtr<T> make_own_ptr(T copy)
{
    return OwnPtr<T>::make(copy);
}

template <class T>
class BorrowPtr
{
private:
    T* _ptr;

public:
    BorrowPtr(const OwnPtr<T>& owner)
    {
        _ptr = owner._ptr;
    }

    ~BorrowPtr()
    {
        _ptr = nullptr;
    }

    BorrowPtr() = delete;
    BorrowPtr(const BorrowPtr<T>& other) = delete;
    BorrowPtr<T> operator=(const BorrowPtr<T>& other) = delete;

    BorrowPtr(BorrowPtr<T>&& other)
    {
        _ptr = other._ptr;
        other._ptr = nullptr;
    }

    BorrowPtr<T> operator=(BorrowPtr<T>&& other)
    {
        if (other != *this) {
            _ptr = other._ptr;
            other._ptr = nullptr;
        }

        return *this;
    }

    T& operator*()
    {
        if (_ptr)
            return *_ptr;
        throw PtrException("Dereferrencing an expired BorrowPtr<T>");
    }

    T* operator->()
    {
        if (_ptr)
            return _ptr;
        throw PtrException("Accessing an expired BorrowPtr<T>");
    }

    bool operator==(const BorrowPtr<T>& other)
    {
        return _ptr == other._ptr;
    }
};

template <class T>
static BorrowPtr<T> make_borrow_ptr(const OwnPtr<T>& ptr)
{
    return BorrowPtr<T>(ptr);
}

template <class T>
class RefPtr
{
private:
    int* _ref;
    T* _ptr;

public:
    RefPtr()
    {
        _ptr = nullptr;
        _ref = nullptr;
    }

    RefPtr(T* ptr)
    {
        _ptr = ptr;
        _ref = new int(1);
    }

    RefPtr(const RefPtr<T>& copy)
    {
        _ptr = copy._ptr;
        _ref = copy._ref;
        (*_ref)++;
    }

    RefPtr<T>& operator=(const RefPtr<T>& copy)
    {
        if (copy != *this) {
            _ptr = copy._ptr;
            _ref = copy._ref;
            (*_ref)++;
        }

        return *this;
    }

    ~RefPtr()
    {
        (*_ref)--;
        if (*_ref == 0)
            delete _ptr;
    }

    T& operator*()
    {
        return *_ptr;
    }

    T* operator->()
    {
        return _ptr;
    }

    T const* get() const
    {
        return _ptr;
    }

    void destroy()
    {
        if (*_ref == 0)
            delete _ptr;
    }

    bool operator==(const RefPtr<T>& other)
    {
        return _ptr == other._ptr;
    }

    template <class... Args>
    static inline RefPtr<T> make(Args&&... args)
    {
        T* t = new T(args...);
        return RefPtr<T>(t);
    }

    static inline RefPtr<T> make(T copy)
    {
        T* t = new T(copy);
        return RefPtr<T>(t);
    }
};

template <class T, class... Args>
static RefPtr<T> make_ref_ptr(Args&&... args)
{
    return RefPtr<T>::make(args...);
}

template <class T>
static RefPtr<T> make_ref_ptr(T copy)
{
    return RefPtr<T>::make(copy);
}

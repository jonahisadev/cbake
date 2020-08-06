#pragma once

template <typename T>
class Bitmask
{
private:
    T _bits;

public:
    Bitmask(T bits = 0);
    ~Bitmask() {}

    void set(int index, int on);
    int get(int index);
    bool contains(T data);
    void reset(T bits = 0);
    bool empty();
};

template <typename T>
Bitmask<T>::Bitmask(T bits)
{
    _bits = bits;
}

template <typename T>
void Bitmask<T>::set(int index, int on)
{
    _bits |= (T)(on << index);
}

template <typename T>
int Bitmask<T>::get(int index)
{
    return (int)((_bits >> index) & 1);
}

template <typename T>
bool Bitmask<T>::contains(T data)
{
    // _bits = 0101
    // data =  0010
    // ret =   0000
    return ((_bits & data) > 0);
}

template <typename T>
void Bitmask<T>::reset(T bits)
{
    _bits = bits;
}

template <typename T>
bool Bitmask<T>::empty()
{
    return (_bits == 0);
}


template <typename T>
static T max(T x, T y)
{
    if (x < y)
        return y;
    else
        return x;
}

template <typename T>
static T min(T x, T y)
{
    if (x > y)
        return y;
    else
        return x;
}
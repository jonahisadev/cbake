#pragma once

#include <iostream>
#include <cstdlib>

#include "JA.h"
#include "util.h"
#include "list.h"
#include "stream.h"

#define STRING_IN_STACK 1024 * 1024

class String
{
private:
    char* _data;
    int _len;

    void copy_c_str(const char* src, int len = 0);
    static void strncpy(const char* src, char* dst, int start, int length);

    explicit String(int size)
    {
        _len = size;
        _data = ALLOC(char, size + 1);
    }
public:
    // Constructors, etc
    String();
    String(const char* data);
    String(const String& other);
    String(String&& other);

    String& operator=(const String& other);
    String& operator=(String&& other);

    ~String();

    // Getters
    inline int length() const { return _len; }
    inline const char* c_str() const { return const_cast<const char*>(_data); }

    // Utility functions
    bool startsWith(const String& needle) const;
    bool endsWith(const String& needle) const;
    bool contains(const String& needle) const;
    String subs(int start, int len) const;
    int indexOf(char c, int count=1) const;
    int indexOf(const String& str, int count=1) const;
    char charAt(int index) const;
    bool equals(const String& other) const;
    const String& safe() const;
    int appears(char c) const;
    bool isEmpty() const;
    bool isNull() const;
    bool isWhitespace() const;

    // Mutation
    String toLowerCase();
    String toUpperCase();
    String append(const String& str);
    String append(const String& str) const;
    String arg(const String& str);
    String arg(int x);
    String reverse();
    String reverse() const;
    List<String> split(char delim) const;
    String insert(int index, const String& str);

    // Operators
    char operator[](int index) const;
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    String operator+(String const& other) const;
    String operator+=(String const& other);

    friend std::ostream& operator<<(std::ostream& os, const String& str)
    {
        return os << (str._data ? str._data : "");
    }

    friend std::istream& operator>>(std::istream& is, String& str)
    {
        // TODO: find a better way to do this
        char buf[STRING_IN_STACK];
        is >> buf;
        str.copy_c_str(buf);
        return is;
    }

    friend OutputStream& operator<<(OutputStream& log, const String& str)
    {
        return log << (str._data ? str._data : "");
    }

    // Conversions
    explicit operator int() const { return strtol(_data, nullptr, 10); }
    explicit operator float() const { return strtof(_data, nullptr); }
    template <typename T>
    T as() { return static_cast<T>(*this); }
    
    // Static members
    static String from(int x)
    {
        char buf[32];
        int i = -1;
        int mod = 10;
        while (x > 0)
        {
            int n = (x % mod);
            x -= n;
            n /= (mod / 10);
            buf[++i] = '0' + n;
            mod *= 10;
        }
        buf[++i] = '\0';
        return String(buf).reverse();
    }

    // Range
    JA_STL_ITERATOR(char);
    stl_iterator begin() { return stl_iterator(_data); }
    stl_iterator end() { return stl_iterator(_data + _len); }
};

#ifdef JA_IMPL
String::String()
    : _data(nullptr), _len(0)
{}

String::String(const char* data)
{
    copy_c_str(data);
}

String::String(const String& other)
{
    copy_c_str(other.c_str(), other.length());
}

String::String(String&& other)
{
    if (this != &other) {
        copy_c_str(other.c_str(), other.length());
    }
}

String::~String()
{
    if (_data)
        FREE(char, _data);
}

String& String::operator=(const String& other)
{
    copy_c_str(other.c_str(), other.length());
    return *this;
}

String& String::operator=(String&& other)
{
    if (this != &other) {
        copy_c_str(other.c_str(), other.length());
    }

    return *this;
}

char String::operator[](int index) const
{
    return charAt(index);
}

bool String::operator==(const String& other) const
{
   return equals(other);
}

bool String::operator!=(const String& other) const
{
   return !equals(other);
}

String String::operator+(String const& other) const
{
    String copy = *this;
    return copy.append(other);
}

String String::operator+=(String const& other)
{
    return append(other);
}

String String::toUpperCase()
{
    char data[_len + 1];
    for (int i = 0; i < _len; i++) {
        char c = _data[i];
        if (c >= 'a' && c <= 'z')
            c -= 32;
        data[i] = c;
    }
    data[_len] = '\0';
    return String(data);
}

String String::toLowerCase()
{
    char data[_len + 1];
    for (int i = 0; i < _len; i++) {
        char c = _data[i];
        if (c >= 'A' && c <= 'Z')
            c += 32;
        data[i] = c;
    }
    data[_len] = '\0';
    return String(data);
}

const String& String::safe() const
{
    return const_cast<const String&>(*this);
}

int String::appears(char c) const
{
    int count = 0;
    for (int i = 0; i < _len; i++)
        if (_data[i] == c) ++count;
    return count;
}

bool String::isEmpty() const
{
    return (_len == 0);
}

bool String::isNull() const
{
    return (_data == nullptr);
}

bool String::isWhitespace() const
{
    for (int i = 0; i < _len; i++) {
        char c = _data[i];
        if (c != ' ' && c != '\n' && c != '\r' && c!= '\t')
            return false;
    }
    return true;
}

bool String::startsWith(const String& needle) const
{
    int len = min<int>(_len, needle.length());
    for (int i = 0; i < len; i++) {
        if (_data[i] != needle._data[i])
            return false;
    }
    return true;
}

bool String::endsWith(const String& needle) const
{
    int off = _len - needle.length();
    if (off < 0) return false;

    for (int i = 0; i < needle.length(); i++) {
        if (_data[i + off] != needle._data[i])
            return false;
    }
    return true;
}

bool String::contains(const String& needle) const
{
    int n = 0;
    for (int i = 0; i < _len; i++) {
        if (needle._data[n] == _data[i])
            n++;
        if (n == needle.length())
            return true;
    }
    return false;
}

String String::subs(int start, int len) const
{
    if (len > _len)
        len = _len;
    String ret(len);
    strncpy(_data, ret._data, start, len);
    return String(ret);
}

int String::indexOf(char c, int count) const
{
    int n = 0;
    while (count > 0) {
        while (_data[n] != c) {
            ++n;
            if (_data[n] == '\0')
                return _len;
        }
        if (--count > 0) ++n;
    }

    return n;
}

// TODO: clean this up homie
int String::indexOf(const String& str, int index) const
{
    int ret = _len;
    int count = 1;
    while (index > 0) {
        while (count <= appears(str[0])) {
            int first = indexOf(str[0], count);
            if (first == _len)
                return _len;
            
            int x = 1;
            while (x < str.length()) {
                if (charAt(first + x) != str[x])
                    break;
                ++x;
            }

            if (x == str.length()) {
                ret = first;
                ++count;
                break;
            } else
                ++count;
        }
        --index;
    }

    return ret;
}

char String::charAt(int index) const
{
    if (index < 0 || index >= _len)
        return '\0';
    return _data[index];
}

bool String::equals(const String& other) const
{
    if (_len != other.length())
        return false;

    for (int i = 0; i < _len; i++) {
        if (_data[i] != other._data[i])
            return false;
    }

    return true;
}

String String::append(const String& str)
{
    char* temp = _data;
    
    _data = ALLOC(char, _len + str.length() + 1);
    for (int i = 0; i < _len; i++)
        _data[i] = temp[i];
    for (int i = _len; i < _len + str.length(); i++)
        _data[i] = str._data[i - _len];
    _data[_len + str.length()] = '\0';
    _len += str.length();

    FREE(char, temp);
    return *this;
}

String String::append(const String& str) const
{
    String cpy = *this;
    cpy.append(str);
    return String(cpy);
}

String String::reverse()
{
    char c;
    int a = 0, b = _len - 1;

    while (a != b && a < b)
    {
        c = _data[a];
        _data[a] = _data[b];
        _data[b] = c;

        a++;
        b--;
    }

    return *this;
}

String String::reverse() const
{
    String cpy = *this;
    cpy.reverse();
    return String(cpy);
}

List<String> String::split(char delim) const
{
    int cc = appears(delim);
    if (cc == 0)
        return List<String>();

    List<String> list(cc);
    String copy = *this;
    auto index = copy.indexOf(delim);
    while (index != copy.length())
    {
        String part = copy.subs(0, index);
        copy = copy.subs(index + 1, copy.length() - index - 1);
        list.push_back(part);

        index = copy.indexOf(delim);
    }
    list.push_back(copy);
    return list;
}

String String::insert(int index, const String& str)
{
    String a = subs(0, index + 1);
    String b = subs(index + 1, length() - index);
    *this = String(a + str + b);
    return *this;
}

String String::arg(const String& str)
{
    int split = indexOf("{}");
    String a = subs(0, split);
    String b = subs(split+2, _len - split - 1);
    return String(a + str + b);
}

String String::arg(int x)
{
    return arg(String::from(x));
}

void String::copy_c_str(const char* src, int len)
{
    if (!len) {
        _len = 0;
        while (src[_len] != '\0')
            _len++;
    } else { _len = len; }

    _data = ALLOC(char, _len + 1);
    for (int i = 0; i < _len; i++)
        _data[i] = src[i];
    _data[_len] = '\0';
}

void String::strncpy(const char* src, char* dst, int start, int length)
{
    for (int i = 0; i < length; i++)
        dst[i] = src[i + start];
    dst[length] = '\0';
}
#endif
#pragma once

#include <exception>

#if defined(__linux__) || defined(__APPLE__)
    #define USER std::getenv("USER")
#elif defined(_WIN32)
    #define USER std::getenv("USERNAME")
#endif

class CBakeException : public std::exception
{
private:
    const char* _what;

public:
    CBakeException(const char* what) : _what(what) {}

    const char* what() const throw()
    {
        return _what;
    }
};
#pragma once

#include <cstdio>

static const char endl = '\n';

//
// OutputStream interface
//

class OutputStream
{
public:
    virtual OutputStream& operator<<(char c) = 0;
    virtual OutputStream& operator<<(const char* str) = 0;
    virtual OutputStream& operator<<(int x) = 0;
    virtual OutputStream& operator<<(unsigned long long x) = 0;
    virtual OutputStream& operator<<(const void* ptr) = 0;
    virtual OutputStream& operator<<(double d) = 0;
};

//
//  File Output (FILE*)
//

class FileOutputStream : public OutputStream
{
protected:
    FILE* _out;

    void close()
    {
        fflush(_out);
        fclose(_out);
    }

    void flush()
    {
        fflush(_out);   
    }
public:
    FileOutputStream(FILE* out)
    {
        _out = out;
    }

    virtual ~FileOutputStream()
    {
        flush();
    }
     
    virtual OutputStream& operator<<(char c);
    virtual OutputStream& operator<<(const char* str);
    virtual OutputStream& operator<<(int x);
    virtual OutputStream& operator<<(unsigned long long x);
    virtual OutputStream& operator<<(const void* ptr);
    virtual OutputStream& operator<<(double d);
};

#ifdef JA_IMPL
OutputStream& FileOutputStream::operator<<(char c)
{
    fputc(c, _out);
    return *this;
}

OutputStream& FileOutputStream::operator<<(const char* str)
{
    fputs(str, _out);
    return *this;
}

OutputStream& FileOutputStream::operator<<(int x)
{
    fprintf(_out, "%d", x);
    return *this;
}

OutputStream& FileOutputStream::operator<<(unsigned long long x)
{
    fprintf(_out, "%llu", x);
    return *this;
}

OutputStream& FileOutputStream::operator<<(const void* ptr)
{
    fprintf(_out, "%p", ptr);
    return *this;
}

OutputStream& FileOutputStream::operator<<(double d)
{
    fprintf(_out, "%.2f", d);
    return *this;
}
#endif
// NOTE: This ONLY works with cstdio platforms

#pragma once

#include "util.h"
#include "string.h"
#include "stream.h"
#include "pointer.h"

#include <cstdio>

class FileWriter : public FileOutputStream
{
public:
    FileWriter(FILE* file) : FileOutputStream(file) {}
    virtual ~FileWriter() { close(); }

    void close() { FileOutputStream::flush(); }
};

class File
{
public:
    constexpr static int ModeNone =   0;
    constexpr static int ModeRead =   1;
    constexpr static int ModeWrite =  2;
    constexpr static int ModeText =   4;
    constexpr static int ModeBinary = 8;

private:
    FILE* _file;
    uint8_t* _buffer;
    String _path;
    Bitmask<uint8_t> _mode;
    int _nbytes;

public:
    File();
    File(const String& path, int mode);
    ~File();

    bool open(const String& path, int mode);
    void close();
    bool read();
    bool write(const String& str);

    FileWriter getWriter();

    inline const String& path() const { return _path; }
    inline int byteCount() const { return _nbytes; }

    String contentsASCII();
    const uint8_t* contentsRaw();
};

#ifdef JA_IMPL
File::File()
{
    _file = nullptr;
    _buffer = nullptr;
    _path = "";
    _mode = ModeNone;
}

File::File(const String& path, int mode)
    : _path(path), _mode(mode)
{
    _file = nullptr;
    _buffer = nullptr;
    open(path, mode);
}

File::~File()
{
    close(); 
}

bool File::open(const String& path, int mode)
{
    // Set mode and verify
    _mode.reset(mode);
    if (_mode.empty() ||
            (_mode.contains(ModeText) && _mode.contains(ModeBinary)) ||
            (_mode.contains(ModeRead) && _mode.contains(ModeWrite)))
        return false;

    // Set path
    _path = path;

    String mode_str;
    if (_mode.contains(ModeRead))
        mode_str = "r";
    else if (_mode.contains(ModeWrite))
        mode_str = "w";
    if (_mode.contains(ModeBinary))
        mode_str += "b";
    
    // Open file
    _file = fopen(_path.c_str(), mode_str.c_str());
    if (_file == nullptr)
        return false;

    // Get file size
    fseek(_file, 0, SEEK_END);
    _nbytes = ftell(_file);
    fseek(_file, 0, SEEK_SET);

    // Done opening file
    return true;
}

void File::close()
{
    if (_file) {
        fclose(_file);
        _file = nullptr;
    }
    if (_buffer) {
        FREE(uint8_t, _buffer);
        _buffer = nullptr;
    }
}

bool File::read()
{
    // Read into buffer
    if (_mode.contains(ModeRead)) {
        _buffer = ALLOC(uint8_t, _nbytes);
        int rbytes = fread(_buffer, 1, _nbytes, _file);
        if (rbytes < 0 || rbytes != _nbytes)
            return false;
    } else { return false; }

    return true;
}

bool File::write(const String& str)
{
    return fwrite(str.c_str(), 1, str.length(), _file) > 0;
}

FileWriter File::getWriter()
{
    return FileWriter(_file);
}

String File::contentsASCII()
{
    if (_buffer && _mode.contains(ModeText)) {
        return String((char*)_buffer);
    }
    return {};
}

const uint8_t* File::contentsRaw()
{
    if (_buffer)
        return const_cast<const uint8_t*>(_buffer);
    return nullptr;
}
#endif
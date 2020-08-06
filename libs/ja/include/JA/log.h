#pragma once

#include "stream.h"
#include <cstdio>

class Logger : public FileOutputStream 
{
public:
    // Colors
    typedef const char* Color;
    constexpr static const char* LightGreen =  "\u001b[32;1m";
    constexpr static const char* LightRed =    "\u001b[31;1m";
    constexpr static const char* LightBlue =   "\u001b[34;1m";
    constexpr static const char* LightWhite =  "\u001b[37;1m";
    constexpr static const char* ResetColor =  "\u001b[0m";

public:
    Logger(FILE* out) : FileOutputStream(out) {}
    ~Logger() {}

    void setColor(Color color) { *this << color; }
};

static Logger jOut(stdout);
static Logger jErr(stderr);
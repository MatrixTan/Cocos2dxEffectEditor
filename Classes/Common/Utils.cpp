//
//  Utils.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/8.
//
//

#include "Utils.hpp"


std::string Utils::stringFormat(const char *format, int size, ...)
{
    char* buffer = new char[size];
    va_list params;
    va_start(params, size);
    vsnprintf(buffer, size, format, params);
    std::string ret = buffer;
    va_end(params);
    delete[] buffer;
    return ret;
}
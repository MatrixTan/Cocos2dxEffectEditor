//
//  Utils.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/8.
//
//

#include "Utils.hpp"
#include <stdarg.h>


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

std::string Utils::jsonFormat(const char *jsonStream)
{
    std::string pretty;
    if (jsonStream == NULL || strlen(jsonStream) == 0){
        return pretty;
    }
    std::string str = std::string(jsonStream);
    bool quoted = false;
    bool escaped = false;
    std::string INDENT = "    ";
    int indent = 0;
    int length = (int) str.length();
    
    for (int i = 0 ; i < length ; i++){
        char ch = str[i];
        switch (ch){
            case '{':
            case '[':
                pretty += ch;
                if (!quoted){
                    pretty += "\n";
                    if (!(str[i+1] == '}' || str[i+1] == ']')){
                        ++indent;
                        for (int j = 0 ; j < indent ; j++){
                            pretty += INDENT;
                        }
                    }
                }
                break;
            case '}':
            case ']':
                if (!quoted){
                    if ((i > 0) && (!(str[i-1] == '{' || str[i-1] == '['))){
                        pretty += "\n";
                        --indent;
                        for (int j = 0 ; j < indent ; j++){
                            pretty += INDENT;
                        }
                    } else if ((i > 0) && ((str[i-1] == '[' && ch == ']') || (str[i-1] == '{' && ch == '}'))){
                        for (int j = 0 ; j < indent ; j++){
                            pretty += INDENT;
                        }
                    }
                }
                pretty += ch;
                break;
            case '"':
                pretty += ch;
                escaped = false;
                if (i > 0 && str[i-1] == '\\'){
                    escaped = !escaped;
                }
                if (!escaped){
                    quoted = !quoted;
                }
                break;
            case ',':
                pretty += ch;
                if (!quoted){
                    pretty += "\n";
                    for (int j = 0 ; j < indent ; j++){
                        pretty += INDENT;
                    }
                }
                break;
            case ':':
                pretty += ch;
                if (!quoted){
                    pretty += " ";
                }
                break;
            default:
                pretty += ch;
                break;
        }
    }
    
    return pretty;
}

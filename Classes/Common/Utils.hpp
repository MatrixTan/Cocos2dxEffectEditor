//
//  Utils.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/8.
//
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <string>

class Utils
{
public:
    static std::string stringFormat(const char* format, int size=256,  ...);
    static std::string jsonFormat(const char* jsonStream);
};

#endif /* Utils_hpp */

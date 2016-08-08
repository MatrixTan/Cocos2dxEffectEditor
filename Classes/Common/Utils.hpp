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

class Utils
{
public:
    static std::string stringFormat(const char* format, int size=256,  ...);
};

#endif /* Utils_hpp */

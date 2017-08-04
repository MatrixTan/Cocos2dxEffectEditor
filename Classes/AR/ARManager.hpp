//
//  ARManager.hpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/4.
//

#ifndef ARManager_hpp
#define ARManager_hpp

#include <stdio.h>
#include "Singleton.hpp"

class ARManager : public Singleton<ARManager>
{
public:
    void startVRView(void);
};

#endif /* ARManager_hpp */

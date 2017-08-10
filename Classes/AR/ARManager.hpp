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
#include "GlobalDefine.hpp"

NS_EE_BEGIN

using namespace cocos2d;

class ARFrameInfo{
public:
    Texture2D* textureY = nullptr;
    Texture2D* textureUV = nullptr;
    
    ARFrameInfo(){
        textureY = new Texture2D();
        textureUV = new Texture2D();
    }
    
    ~ARFrameInfo(){
        if(textureY){
            textureY->release();
        }
        if(textureUV){
            textureUV->release();
        }
    }
};

class ARManager : public Singleton<ARManager>
{
public:
    void startVRView(void);
    ARFrameInfo* getARFrameInfo(void);
    
private:
    void* mARView = nullptr;
    ARFrameInfo *mARFrameInfo = nullptr;
};

NS_EE_END

#endif /* ARManager_hpp */

//
//  NodeSingleton.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/8.
//
//

#ifndef NodeSingleton_hpp
#define NodeSingleton_hpp

#include <stdio.h>
#include <cocos2d.h>

template<class T>
class NodeSingleton
{
private:
    NodeSingleton(NodeSingleton const&){};

public:
    static T* getInstance()
    {
        if(!mInstance)
        {
            mInstance = T::create();
        }
        return mInstance;
    }
    
    ~NodeSingleton()
    {
        mInstance = nullptr;
    } 

    
protected:
    static T *mInstance;
    NodeSingleton(){};
};

template<class T>
T* NodeSingleton<T>::mInstance = nullptr;



#endif /* NodeSingleton_hpp */

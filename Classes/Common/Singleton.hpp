//
//  Singleton.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/8.
//
//

#ifndef Singleton_hpp
#define Singleton_hpp

#include <stdio.h>

template<class T>
class Singleton
{
public:
    static T* getInstance();
    static void destroy();
    
private:
    Singleton(Singleton const&){};
    Singleton& operatior=(Singleton const&){};
    
protected:
    static T *mInstance;
};

template<class T>
T* Singleton<T>::mInstance = nullptr;


template<class T>
T* Singleton<T>::getInstance()
{
    if(!mInstance)
    {
        Singleton<T>::mInstance = new T();
    }
    return mInstance;
}

template<class T>
void Singleton<T>::destroy()
{
    delete Singleton<T>::mInstance;
    Singleton<T>::mInstance = nullptr;
}

#endif /* Singleton_hpp */

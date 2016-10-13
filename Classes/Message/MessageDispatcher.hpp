//
//  MessageDispatcher.hpp
//  EffectEditor
//
//  Created by Matrix on 16/9/2.
//
//

#ifndef MessageDispatcher_hpp
#define MessageDispatcher_hpp

#include <stdio.h>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include "GlobalDefine.hpp"
#include "Singleton.hpp"
#include "MessageParam.hpp"
#include "MessageListener.hpp"

NS_EE_BEGIN

typedef std::function<void(void* ,MessageParam*)> MessageFunction;

class MessageListenerStruct
{
public:
    MessageListener* listener;
    MessageFunction func;
};

class MessageDispatcher: public Singleton<MessageDispatcher>
{
public:
    MessageDispatcher();
    
    void addListener(std::string message, MessageListener* listener, MessageFunction func);
    void removeListener(std::string message, MessageListener* listener);
    void notifyMessage(std::string message, void* sender, MessageParam* param);
    
private:
    typedef std::vector<MessageListenerStruct*> ListenerVector;
    typedef std::map<std::string, ListenerVector> ListenerMap;
    
    ListenerMap mListenerMap;
};

NS_EE_END

#endif /* MessageDispatcher_hpp */

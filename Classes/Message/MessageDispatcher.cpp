//
//  MessageDispatcher.cpp
//  EffectEditor
//
//  Created by Matrix on 16/9/2.
//
//

#include "MessageDispatcher.hpp"

NS_EE_BEGIN

MessageDispatcher::MessageDispatcher()
{
}

void MessageDispatcher::addListener(std::string message, MessageListener* listener, MessageFunction func)
{
    ListenerMap::iterator messageVector = mListenerMap.find(message);
    if(messageVector == mListenerMap.end()){
        ListenerVector vect;
        mListenerMap[message] = vect;
    }
    MessageListenerStruct *pListener = new MessageListenerStruct();
    pListener->listener = listener;
    pListener->func = func;
    mListenerMap[message].push_back(pListener);
}

void MessageDispatcher::removeListener(std::string message, MessageListener* listener)
{
    ListenerMap::iterator messageVector = mListenerMap.find(message);
    if(messageVector != mListenerMap.end())
    {
        ListenerVector::iterator messageStruct = messageVector->second.begin();
        for(; messageStruct != messageVector->second.end(); ){
            if((*messageStruct)->listener == listener){
                delete (*messageStruct);
                messageStruct = messageVector->second.erase(messageStruct);
            }else{
                messageStruct++;
            }
        }
    }
}

void MessageDispatcher::notifyMessage(std::string message, void *sender, ee::MessageParam *param)
{
    ListenerMap::iterator messageVector = mListenerMap.find(message);
    if(messageVector != mListenerMap.end()){
        for( auto messageListener : messageVector->second){
            messageListener->func(sender, param);
        }
    }
}


NS_EE_END
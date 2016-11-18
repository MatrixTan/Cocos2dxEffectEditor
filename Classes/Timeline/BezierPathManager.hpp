//
//  BeizerPathManager.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/10/21.
//
//

#ifndef BeizerPathManager_hpp
#define BeizerPathManager_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Singleton.hpp"
#include <cocos2d.h>
#include <list>

NS_EE_BEGIN
USING_NS_CC;

class BezierPoint
{
public:
    Vec2 pos;
    Vec2 control1;
    Vec2 control2;
    
    BezierPoint(){}
    
    BezierPoint(const BezierPoint& p){
        pos = p.pos;
        control1 = p.control1;
        control2 = p.control2;
    }
    
    BezierPoint& operator = (const BezierPoint& p){
        pos = p.pos;
        control1 = p.control1;
        control2 = p.control2;
        return *this;
    }
};

typedef std::list<BezierPoint*> BezierPointList;
typedef std::map<std::string, BezierPointList> BezierPointMap;

class BezierPathManager : public Singleton<BezierPathManager>
{
public:
    bool loadBezierPath(const std::string& pathFile);
    BezierPointList getBezierPath(const std::string& pathKey);
    //bezier path in config files is based on (0, 0) point, so the 
    Sequence* getBezierPathSequence(const std::string& pathFile, float duration, Vec2 from, Vec2 to, bool rotate = false);
    bool saveBezierPath(BezierPointList& list, const std::string& filePath);
    static void clearList(BezierPointList& list);
private:
    
private:
    
    BezierPointMap mCacheMap;
};
    
    NS_EE_END
    
#endif /* BeizerPathManager_hpp */

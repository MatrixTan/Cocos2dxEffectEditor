//
//  ActionDrawBezierPath.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/18.
//
//

#include "ActionDrawBezierPath.hpp"

NS_EE_BEGIN

ActionDrawBezierPath* ActionDrawBezierPath::create(float duration,
                                                   const BezierPointList& list,
                                                   float width,
                                                   Color4F color)
{
    ActionDrawBezierPath* pRet = new ActionDrawBezierPath();
    if (pRet != nullptr && pRet->initWithDuration(duration, list, width, color))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool ActionDrawBezierPath::initWithDuration(float duration, const BezierPointList& list, float width, Color4F color)
{
    _duration = duration;
    mPoints = list;
    mWidth = width;
    mColor = color;
    return true;
}

void ActionDrawBezierPath::startWithTarget(Node* pTarget)
{
    mDrawNode = static_cast<DrawNode*>(pTarget);
    CCASSERT(mDrawNode != nullptr, "ActionDrawBezierPath HAVE TO be running on the DrawNode");
    mDrawTime = 0.0f;
    BezierPoint *lastPoint = nullptr;
    
    for(auto point : mPoints){
        if(lastPoint == nullptr){
            lastPoint = point;
            continue;
        }else{
            float distance = lastPoint->pos.distance(lastPoint->control2);
            distance += lastPoint->control2.distance(point->control1);
            distance += point->control1.distance(point->pos);
            mLengths.push_back(distance);
            mAllLength += distance;
            lastPoint = point;
        }
    }
    
}

void ActionDrawBezierPath::step(float time)
{
    mDrawNode->clear();
    mDrawTime += time;
    float percent = mDrawTime / _duration;
    int index = 0;
    float sumLength = 0.0f;
    BezierPointList::iterator iter = mPoints.begin();
    for(float length : mLengths){
        float nextSumLength = sumLength + length;
        auto currentPoint = *iter;
        auto nextPoint = *(++iter);
        if(percent > nextSumLength / mAllLength){
            index++;
            
            drawBezierLine(currentPoint, nextPoint, 1.0f);
        }else{
            float t = (percent - sumLength / mAllLength)/(length / mAllLength);
            drawBezierLine(currentPoint, nextPoint, t);
            break;
        }
        sumLength = nextSumLength;
    }
}

void ActionDrawBezierPath::drawBezierLine(BezierPoint *p1, ee::BezierPoint *p2, float percent)
{
    int segments = 100;
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0;
    for (unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 3) * p1->pos.x + 3.0f * powf(1 - t, 2) * t * p1->control2.x + 3.0f * (1 - t) * t * t * p2->control1.x + t * t * t * p2->pos.x;
        vertices[i].y = powf(1 - t, 3) * p1->pos.y + 3.0f * powf(1 - t, 2) * t * p1->control2.y + 3.0f * (1 - t) * t * t * p2->control1.y + t * t * t * p2->pos.y;
        t += percent / segments;
    }
    
    if(percent >= 1.0f){
        vertices[segments].x = p2->pos.x;
        vertices[segments].y = p2->pos.y;
    }else{
        vertices[segments].x = vertices[segments-1].x;
        vertices[segments].y = vertices[segments-1].y;
    }
    mDrawNode->drawPoints(vertices, segments+1, mWidth, mColor);
    CC_SAFE_DELETE_ARRAY(vertices);
}

ActionDrawBezierPath* ActionDrawBezierPath::clone() const
{
    return ActionDrawBezierPath::create(_duration, mPoints, mWidth, mColor);
}

NS_EE_END

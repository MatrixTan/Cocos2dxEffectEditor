//
//  BeizerPathManager.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/10/21.
//
//

#include "BezierPathManager.hpp"
#include <json/document.h>
#include <json/writer.h>
#include <json/stringbuffer.h>
#include "ActionEx.hpp"

NS_EE_BEGIN

bool BezierPathManager::loadBezierPath(const std::string &pathFile)
{
    std::string pathString = FileUtils::getInstance()->getStringFromFile(pathFile);
    if(pathString.length() <= 0){
        return false;
    }
    
    rapidjson::Document document;
    document.Parse(pathString.c_str());
    if(document.HasParseError()){
        return  false;
    }
    
    rapidjson::Value& root = document["data"];
    BezierPointList list;
    for(int i=0; i<root.Size(); i++){
        BezierPoint* pPoint = new BezierPoint();
        pPoint->pos.x = root[i]["pos"]["x"].GetDouble();
        pPoint->pos.y = root[i]["pos"]["y"].GetDouble();
        pPoint->control1.x = root[i]["control1"]["x"].GetDouble();
        pPoint->control1.y = root[i]["control1"]["y"].GetDouble();
        pPoint->control2.x = root[i]["control2"]["x"].GetDouble();
        pPoint->control2.y = root[i]["control2"]["y"].GetDouble();
        list.push_back(pPoint);
    }
    
    if(mCacheMap.find(pathFile) != mCacheMap.end()){
        clearList(mCacheMap[pathFile]);
    }
    mCacheMap[pathFile] = list;
    return true;
}

Sequence* BezierPathManager::getBezierPathSequence(const std::string& pathFile, float duration, Vec2 from, Vec2 to, bool rotate)
{
    if(mCacheMap.find(pathFile) == mCacheMap.end()){
        loadBezierPath(pathFile);
    }
    BezierPointList& list = mCacheMap[pathFile];
    BezierPoint* lastPoint = nullptr;
    
    //calculate the duration of each tile. using the distance between control points for simple.
    float allDistance = 0.0f;
    std::vector<float> distances;
    for(BezierPointList::iterator iter = list.begin(); iter != list.end(); iter++){
        if(lastPoint != nullptr){
            float distance = 0.0f;
            distance += lastPoint->pos.distance(lastPoint->control2);
            distance += lastPoint->control2.distance((*iter)->control1);
            distance += (*iter)->control1.distance((*iter)->pos);
            allDistance += distance;
            distances.push_back(distance);
        }
        lastPoint = *iter;
    }
    
    lastPoint = nullptr;
    Vector<FiniteTimeAction*> actions;
    int index = 0;
    for(BezierPointList::iterator iter = list.begin(); iter != list.end(); iter++){
        if(lastPoint != nullptr){
            ccBezierConfig config;
            config.controlPoint_1 = lastPoint->control2 + from;
            config.controlPoint_2 = (*iter)->control1 + from;
            config.endPosition = (*iter)->pos + from;
            if(rotate){
                auto oneAction = BezierRotateTo::create(duration * distances[index++] / allDistance, config);
                actions.pushBack(oneAction);
            }else{
                auto oneAction = BezierTo::create(duration * distances[index++] / allDistance, config);
                actions.pushBack(oneAction);
            }
            
        }
        lastPoint = *iter;
    }
    return Sequence::create(actions);
}


bool BezierPathManager::saveBezierPath(BezierPointList &list, const std::string &filePath)
{
    if(list.size() > 0){
        rapidjson::Document document;
        document.SetObject();
        std::list<BezierPoint*>::iterator iter = list.begin();
        Vec2 orgin = (*iter)->pos;
        rapidjson::Value pointArray(rapidjson::kArrayType);
        for(; iter != list.end(); iter++)
        {
            rapidjson::Value point(rapidjson::kObjectType);
            rapidjson::Value pos(rapidjson::kObjectType);
            pos.AddMember("x", (*iter)->pos.x - orgin.x, document.GetAllocator());
            pos.AddMember("y", (*iter)->pos.y - orgin.y, document.GetAllocator());
            point.AddMember("pos", pos, document.GetAllocator());
            
            rapidjson::Value control1(rapidjson::kObjectType);
            control1.AddMember("x", (*iter)->control1.x - orgin.x, document.GetAllocator());
            control1.AddMember("y", (*iter)->control1.y - orgin.y, document.GetAllocator());
            point.AddMember("control1", control1, document.GetAllocator());
            
            rapidjson::Value control2(rapidjson::kObjectType);
            control2.AddMember("x", (*iter)->control2.x - orgin.x, document.GetAllocator());
            control2.AddMember("y", (*iter)->control2.y - orgin.y, document.GetAllocator());
            point.AddMember("control2", control2, document.GetAllocator());
            
            pointArray.PushBack(point, document.GetAllocator());
        }
        
        document.AddMember("data", pointArray, document.GetAllocator());
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        return FileUtils::getInstance()->writeStringToFile(buffer.GetString(), filePath);
    }
    return false;

}

void BezierPathManager::clearList(BezierPointList &list)
{
    for(BezierPointList::iterator iter = list.begin(); iter != list.end(); iter++){
        delete (*iter);
    }
    list.clear();
}

NS_EE_END
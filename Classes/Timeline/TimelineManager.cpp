//
//  TimelineManager.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/5.
//
//

#include "TimelineManager.hpp"

NS_EE_BEGIN

Timeline* TimelineManager::getTimelineFromJson(const rapidjson::Value &value)
{
    std::string type = value["type"].GetString();
    if(type == "SEQUENCE"){
        auto sequenceTimeline = new(std::nothrow) TimelineSequence();
        const rapidjson::Value& children = value["children"];
        for(int i=0; i<children.Size(); i++){
            sequenceTimeline->children.push_back(getTimelineFromJson(children[i]));
        }
        return sequenceTimeline;
    }else if(type == "MOVE_BY"){
        auto moveByTimeline = new(std::nothrow) TimelineMoveBy();
        moveByTimeline->duration = value["duration"].GetDouble();
        moveByTimeline->x = value["x"].GetDouble();
        moveByTimeline->y = value["y"].GetDouble();
        return moveByTimeline;
    }else if(type == "REPEAT"){
        auto repeatTimeline = new(std::nothrow) TimelineRepeat();
        repeatTimeline->repeat = value["repeat"].GetInt();
        repeatTimeline->child = getTimelineFromJson(value["child"]);
        return repeatTimeline;
    }else if(type == "SPAWN"){
        auto spawnTimeline = new(std::nothrow) TimelineSpawn();
        const rapidjson::Value& children = value["children"];
        for(int i=0; i<children.Size(); i++){
            spawnTimeline->children.push_back(getTimelineFromJson(children[i]));
        }
        return spawnTimeline;
    }else if(type == "SCALE_TO"){
        auto scaleTo = new(std::nothrow) TimelineScaleTo();
        scaleTo->duration = value["duration"].GetDouble();
        scaleTo->x = value["x"].GetDouble();
        scaleTo->y = value["y"].GetDouble();
        return scaleTo;
    }else if(type == "ROTATE_BY"){
        auto rotateBy = new(std::nothrow) TimelineRotateBy();
        rotateBy->duration = value["duration"].GetDouble();
        rotateBy->angle = value["angle"].GetDouble();
        return rotateBy;
    }else if(type == "EASE"){
        auto ease = new(std::nothrow) TimelineEase();
        ease->rate = value["rate"].GetDouble();
        ease->child = getTimelineFromJson(value["child"]);
        return ease;
    }else if(type == "EASE_ELASTIC"){
        std::string trend = value["trend"].GetString();
        TimelineElastic *elastic = nullptr;
        if(trend == "in"){
            elastic = new(std::nothrow) TimelineElasticIn();
        }else if(trend == "out"){
            elastic = new(std::nothrow) TimelineElasticOut();
        }else if(trend == "inout"){
            elastic = new(std::nothrow) TimelineElasticInOut();
        }
        if(elastic){
            elastic->p = value["p"].GetDouble();
            elastic->child = getTimelineFromJson(value["child"]);
        }
        return elastic;
    }else if(type == "FADE"){
        auto fade = new(std::nothrow) TimelineFade();
        fade->duration = value["duration"].GetDouble();
        fade->to = value["to"].GetDouble();
        return fade;
    }else if(type == "DELAY"){
        auto delay = new(std::nothrow) TimelineDelay();
        delay->delay = value["delay"].GetDouble();
        return delay;
    }
    return nullptr;
}

Timeline* TimelineManager::getTimelineFromFile(const std::string &path, bool cache)
{
    if(cache){
        if(mMap.find(path) != mMap.end()){
            return mMap[path];
        }
    }
    std::string timelineString = FileUtils::getInstance()->getStringFromFile(path);
    rapidjson::Document document;
    document.Parse<0>(timelineString.c_str());
    if(document.HasParseError()){
        CCASSERT(false, "timeline parse error");
        return nullptr;
    }
    
    rapidjson::Value& root = document["data"];
    Timeline* timeline = getTimelineFromJson(root);
    if(cache){
        mMap[path] = timeline;
    }
    return timeline;
}

NS_EE_END

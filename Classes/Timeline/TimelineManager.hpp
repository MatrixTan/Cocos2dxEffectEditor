//
//  TimelineManager.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/5.
//
//

#ifndef TimelineManager_hpp
#define TimelineManager_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Common/Singleton.hpp"
#include "Timeline.hpp"
#include <json/rapidjson.h>
#include <json/document.h>
#include <map>

#include "Timeline.hpp"
#include "TimelineSpawn.hpp"
#include "TimelineMoveBy.hpp"
#include "TimelineRepeat.hpp"
#include "TimelineScaleTo.hpp"
#include "TimelineRotateBy.hpp"
#include "TimelineSequence.hpp"
#include "TimelineEase.hpp"
#include "TimelineElastic.hpp"
#include "TimelineFade.hpp"
#include "TimelineDelay.hpp"


NS_EE_BEGIN

class TimelineManager : public Singleton<TimelineManager>
{
public:
    Timeline* getTimelineFromJson(const rapidjson::Value &value);
    Timeline* getTimelineFromFile(const std::string& path, bool cache = false);
    
private:
    typedef std::map<std::string, Timeline*> TimelineMap;
    
    TimelineMap mMap;
};

NS_EE_END

#endif /* TimelineManager_hpp */

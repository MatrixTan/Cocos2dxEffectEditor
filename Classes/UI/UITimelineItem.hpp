//
//  UITimelineItem.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/31.
//
//

#ifndef UITimelineItem_hpp
#define UITimelineItem_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "TimelineConfig.hpp"

NS_EE_BEGIN

using namespace cocos2d;
using namespace cocos2d::ui;

class UITimelineItem
{
public:
    UITimelineItem(Widget* widget, Timeline* config);
};

NS_EE_END

#endif /* UITimelineItem_hpp */

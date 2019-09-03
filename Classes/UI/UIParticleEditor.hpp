
#ifndef UI_PARTICLE_EDITOR_HPP
#define UI_PARTICLE_EDITOR_HPP

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "Common/NodeSingleton.hpp"
#include "ui/CocosGUI.h"

NS_EE_BEGIN

USING_NS_CC;
using namespace ui;

class UIParticleEditor : public NodeSingleton<UIParticleEditor>, public Layer
{
public:
	virtual bool init() override;
	CREATE_FUNC(UIParticleEditor);

private:
	void onButtonTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type);
};

NS_EE_END

#endif // !UI_PARTICLE_EDITOR_HPP

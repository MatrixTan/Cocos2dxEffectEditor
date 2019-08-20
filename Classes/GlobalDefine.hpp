//
//  GlobalDefine.h
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#ifndef GlobalDefine_h
#define GlobalDefine_h

#define NS_EE_BEGIN		namespace ee{
#define NS_EE_END		}
#define USING_NS_EE		using namespace ee


NS_EE_BEGIN

enum class LAYER_ZORDER
{
    MAIN = 100,
    POST_RENDER = 200
};

enum class SPRITE_ZORDER
{
    BACK_GROUND = 0,
    SPRITE = 100,
    MASK = 200,
    UI = 2000
};

NS_EE_END

#endif /* GlobalDefine_h */

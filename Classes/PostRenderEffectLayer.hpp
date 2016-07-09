//
//  PostRenderEffectLayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/8.
//
//

#ifndef PostRenderEffectLayer_hpp
#define PostRenderEffectLayer_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "NodeSingleton.hpp"
#include "GlobalDefine.hpp"

using namespace cocos2d;

NS_EE_BEGIN

class PostRenderEffectLayer : public NodeSingleton<PostRenderEffectLayer> , public Layer
{
public:
    PostRenderEffectLayer();
    ~PostRenderEffectLayer();
    
    virtual bool init(void) override;
    virtual void draw(Renderer *render, const Mat4& transform, uint32_t flags) override;
    
    CREATE_FUNC(PostRenderEffectLayer);
private:
    
    void onDraw(const Mat4 &transfrom, uint32_t flags);
    
    RenderTexture *mRenderTexture;
    Texture2D* mTexture;
    Sprite *mSprite;
    CustomCommand mRenderCommand;
    V3F_C4B_T2F_Quad mQuad;
    
};

NS_EE_END

#endif /* PostRenderEffectLayer_hpp */

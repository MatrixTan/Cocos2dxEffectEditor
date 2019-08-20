//
//  UIDrawView.cpp
//  EffectEditor
//
//  Created by Matrix on 16/10/18.
//
//

#include "UIDrawView.hpp"
#include "MainLayer.hpp"
#include "Common/PlatformAdapter.h"


NS_EE_BEGIN

UIDrawView::UIDrawView(Node* root)
{
    mRoot = static_cast<ui::Widget*>(root);
    mRoot->addTouchEventListener(CC_CALLBACK_2(UIDrawView::onTouchEvent, this));
    mDrawNode = DrawNode::create();
    MainLayer::getInstance()->addChild(mDrawNode);
    mCurrentBezierPoint = nullptr;
    mSelectType = E_SELECT_POINT_TYPE::NONE;
    mSelectedPoint = nullptr;
    
    auto *pSaveButton = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(mRoot, "bt_save"));
    pSaveButton->addTouchEventListener(CC_CALLBACK_2(UIDrawView::onSaveEvent, this));
    auto *pLoadButton = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(mRoot, "bt_load"));
    pLoadButton->addTouchEventListener(CC_CALLBACK_2(UIDrawView::onLoadEvent, this));
    auto *pClearButton = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(mRoot, "bt_clear"));
    pClearButton->addTouchEventListener(CC_CALLBACK_2(UIDrawView::onClearEvent, this));
    
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode , Event *event){
        if(keyCode == EventKeyboard::KeyCode::KEY_DELETE){
            if(mSelectedPoint != nullptr){
                for(std::list<BezierPoint*>::iterator iter = mBezierPoints.begin(); iter != mBezierPoints.end(); iter++){
                    if((*iter) == mSelectedPoint){
                        mBezierPoints.remove(*iter);
                        mSelectedPoint = nullptr;
                        mSelectType = E_SELECT_POINT_TYPE::NONE;
                        reDraw();
                        return;
                    }
                }
            }
        }else if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
            if(mSelectType != E_SELECT_POINT_TYPE::NONE){
                mSelectedPoint = nullptr;
                mSelectType = E_SELECT_POINT_TYPE::NONE;
            }else{
                if(mBezierPoints.size() > 0){
                    mBezierPoints.pop_back();
                }
            }
            
            reDraw();
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyListener, 1);
}


void UIDrawView::onTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED){
        auto pos = mRoot->getTouchEndPosition();
        if(mCurrentBezierPoint != nullptr){
            mBezierPoints.push_back(mCurrentBezierPoint);
            mCurrentBezierPoint = nullptr;
            reDraw();
        }
    }else if(type == ui::Widget::TouchEventType::BEGAN){
        auto pos = mRoot->getTouchBeganPosition();
        if(checkSelectPoint(pos)){
            
        }else{
            if(mCurrentBezierPoint == nullptr){
                mCurrentBezierPoint = new BezierPoint();
                mCurrentBezierPoint->pos = pos;
                mCurrentBezierPoint->control1 = mCurrentBezierPoint->pos;
                mCurrentBezierPoint->control2 = mCurrentBezierPoint->pos;
            }
        }
        
        reDraw();
        
    }else if(type == ui::Widget::TouchEventType::MOVED){
        auto pos = mRoot->getTouchMovePosition();
        if(mSelectType == E_SELECT_POINT_TYPE::SRC){
            auto offset = mSelectedPoint->control1 - mSelectedPoint->pos;
            mSelectedPoint->pos = pos;
            mSelectedPoint->control1 = pos + offset;
            mSelectedPoint->control2 = pos - offset;
        }else if(mSelectType == E_SELECT_POINT_TYPE::CONTROL1){
            mSelectedPoint->control1 = pos;
            auto offset = mSelectedPoint->control1 - mSelectedPoint->pos;
            mSelectedPoint->control2 = mSelectedPoint->pos - offset;
        }else if(mSelectType == E_SELECT_POINT_TYPE::CONTROL2){
            mSelectedPoint->control2 = pos;
            auto offset = mSelectedPoint->control2 - mSelectedPoint->pos;
            mSelectedPoint->control1 = mSelectedPoint->pos - offset;
        }else if(mCurrentBezierPoint != nullptr){
            auto offset = pos - mCurrentBezierPoint->pos;
            mCurrentBezierPoint->control2 = pos;
            mCurrentBezierPoint->control1 = mCurrentBezierPoint->pos - offset;
        }
        reDraw();
    }
}

bool UIDrawView::checkSelectPoint(const Vec2& pos)
{
    std::list<BezierPoint*>::iterator iter = mBezierPoints.begin();
    for(; iter != mBezierPoints.end(); iter++)
    {
        if(pos.distance((*iter)->pos) < 3.0f){
            mSelectType = E_SELECT_POINT_TYPE::SRC;
            mSelectedPoint = (*iter);
            return true;
        }else if(pos.distance((*iter)->control1) < 3.0f){
            mSelectType = E_SELECT_POINT_TYPE::CONTROL1;
            mSelectedPoint = (*iter);
            return true;
        }else if(pos.distance((*iter)->control2) < 3.0f){
            mSelectType = E_SELECT_POINT_TYPE::CONTROL2;
            mSelectedPoint = (*iter);
            return true;
        }
    }
    mSelectType = E_SELECT_POINT_TYPE::NONE;
    mSelectedPoint = nullptr;
    return false;
}

void UIDrawView::reDraw()
{
    mDrawNode->clear();
    BezierPoint *lastPoint = nullptr;
    std::list<BezierPoint*>::iterator iter = mBezierPoints.begin();
    for(; iter != mBezierPoints.end(); iter++)
    {
        if(lastPoint == nullptr){
            lastPoint = *iter;
        }else{
            mDrawNode->drawCubicBezier(lastPoint->pos, lastPoint->control2, (*iter)->control1, (*iter)->pos, 100, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
            lastPoint = *iter;
        }
        mDrawNode->drawDot((*iter)->pos, 3, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
        mDrawNode->drawDot((*iter)->control1, 2, Color4F(1.0f, 1.0f, 0.0f, 1.0f));
        mDrawNode->drawDot((*iter)->control2, 2, Color4F(1.0f, 1.0f, 0.0f, 1.0f));
        mDrawNode->drawLine((*iter)->pos, (*iter)->control1, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        mDrawNode->drawLine((*iter)->pos, (*iter)->control2, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    }

    if(mCurrentBezierPoint != nullptr){
        mDrawNode->drawDot(mCurrentBezierPoint->pos, 3, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
        mDrawNode->drawDot(mCurrentBezierPoint->control1, 2, Color4F(1.0f, 1.0f, 0.0f, 1.0f));
        mDrawNode->drawDot(mCurrentBezierPoint->control2, 2, Color4F(1.0f, 1.0f, 0.0f, 1.0f));
        mDrawNode->drawLine(mCurrentBezierPoint->pos, mCurrentBezierPoint->control1, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        mDrawNode->drawLine(mCurrentBezierPoint->pos, mCurrentBezierPoint->control2, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        
        if(lastPoint != nullptr){
            mDrawNode->drawCubicBezier(lastPoint->pos, lastPoint->control2, mCurrentBezierPoint->control1, mCurrentBezierPoint->pos, 100, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }
    
    if(mSelectType == E_SELECT_POINT_TYPE::SRC){
        mDrawNode->drawDot(mSelectedPoint->pos, 5, Color4F(0.0f, 1.0f, 1.0f, 1.0f));
    }else if(mSelectType == E_SELECT_POINT_TYPE::CONTROL1){
        mDrawNode->drawDot(mSelectedPoint->control1, 5, Color4F(0.0f, 1.0f, 1.0f, 1.0f));
    }else if(mSelectType == E_SELECT_POINT_TYPE::CONTROL2){
        mDrawNode->drawDot(mSelectedPoint->control2, 5, Color4F(0.0f, 1.0f, 1.0f, 1.0f));
    }

}

void UIDrawView::setVisible(bool visible)
{
    mRoot->setVisible(visible);
}

void UIDrawView::onSaveEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED){
        savePath();
    }
}

void UIDrawView::onLoadEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED){
        std::string filePath = PlatformAdapter::getFilePath("bezier");
        if(filePath.length() > 0){
            if(BezierPathManager::getInstance()->loadBezierPath(filePath)){
                BezierPointList list = BezierPathManager::getInstance()->getBezierPath(filePath);
                if(list.size() > 0){
                    
                    Size screenSize = Director::getInstance()->getWinSize() * 0.5;
                    for(BezierPoint* orPoint : list){
                        orPoint->pos += Vec2(screenSize.width, screenSize.height);
                        orPoint->control1 += Vec2(screenSize.width, screenSize.height);
                        orPoint->control2 += Vec2(screenSize.width, screenSize.height);
                    }
                    
                    clear();
                    mBezierPoints = list;
                    reDraw();
                }
            }
        }
    }
}

void UIDrawView::onClearEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED){
        clear();
    }
}

void UIDrawView::savePath()
{
    std::string savePath = PlatformAdapter::getSaveFilePath("bezier");
    if(savePath.size() > 0){
        BezierPathManager::getInstance()->saveBezierPath(mBezierPoints, savePath);
    }
}

void UIDrawView::clear()
{
    BezierPathManager::clearList(mBezierPoints);
    mSelectedPoint = nullptr;
    mCurrentBezierPoint = nullptr;
    mSelectType = E_SELECT_POINT_TYPE::NONE;
    reDraw();
}

NS_EE_END
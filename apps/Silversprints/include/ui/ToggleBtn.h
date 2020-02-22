#pragma once

#include "cinder/app/App.h"
#include "cinder/Signals.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Signals.h"

#include "data/Model.h"

using ToggleBtnRef = std::shared_ptr<class ToggleBtn>;

class ToggleBtn {
  public:
    enum class TOGGLE_SIDE {
        LEFT,
        RIGHT
    };
    
    ToggleBtn(std::string choice1, std::string choice2, ci::gl::TextureFontRef font, ci::vec2 position = ci::vec2(0,0));
    ~ToggleBtn();
    
    void setColors( ci::ColorA bg, ci::ColorA text);
    void setSelected(TOGGLE_SIDE side) { bLeftActive = (side == TOGGLE_SIDE::LEFT) ? true : false; }
    TOGGLE_SIDE getSelected(){ return bLeftActive ? TOGGLE_SIDE::LEFT : TOGGLE_SIDE::RIGHT; }
    void draw();
    
    ci::signals::Signal<void(TOGGLE_SIDE)> sOnToggleChange;
    
  private:
    void onMouseDown(ci::app::MouseEvent event);
    
    ci::vec2                            mPos;
    ci::signals::ScopedConnection       mMouseDownCb;
    ci::gl::TextureFontRef              mFont;
	float								mFontScale = 1.0f;
    
    std::string     mChoice1, mChoice2;
    ci::Rectf       mRectL, mRectR;
    bool            bLeftActive = true;
    ci::vec2        mTextPadding;
    
    ci::Color       mBgColor, mTextColor;
};

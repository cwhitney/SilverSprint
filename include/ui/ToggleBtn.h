#pragma once

#include "cinder/app/App.h"
#include "cinder/Signals.h"
#include "cinder/gl/TextureFont.h"

#ifdef __linux
    //linux
    #include "../data/GFXGlobal.h"
#else
    // Windows & OSX
    #include "data/GFXGlobal.h"
#endif

using ToggleBtnRef = std::shared_ptr<class ToggleBtn>;

class ToggleBtn {
  public:
    ToggleBtn(std::string choice1, std::string choice2, ci::gl::TextureFontRef font, ci::vec2 position = ci::vec2(0,0));
    ~ToggleBtn();

    void setColors( ci::ColorA bg, ci::ColorA text);
    void setSelected(int side) { bLeftActive = (side) ? false : true; }
    int getSelected(){ return bLeftActive ? 0 : 1; }
    void draw();

  private:
    void onMouseDown(ci::app::MouseEvent event);

    ci::vec2                            mPos;
    ci::signals::ScopedConnection       mMouseDownCb;
    ci::gl::TextureFontRef              mFont;

    std::string     mChoice1, mChoice2;
    ci::Rectf       mRectL, mRectR;
    bool            bLeftActive = true;
    ci::vec2        mTextPadding;

    ci::Color       mBgColor, mTextColor;
};

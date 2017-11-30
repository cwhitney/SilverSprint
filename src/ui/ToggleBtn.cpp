#include "ui/ToggleBtn.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ToggleBtn::ToggleBtn(std::string choice1, std::string choice2, ci::gl::TextureFontRef font, ci::vec2 position)
{
    mPos = position;
    mFont = font;
    mChoice1 = choice1;
    mChoice2 = choice2;
    auto window = getWindow();
    mMouseDownCb = window->getSignalMouseDown().connect( std::bind(&ToggleBtn::onMouseDown, this, std::placeholders::_1) );
    
    vec2 c1 = mFont->measureString(choice1);
    vec2 c2 = mFont->measureString(choice1);
    
    float height = max(c1.y, c2.y);
    mTextPadding = vec2(20, 14);
    
    mRectL = Rectf(mPos.x, mPos.y, mPos.x + c1.x + mTextPadding.x * 2.0f, mPos.y + height + mTextPadding.y * 2.0f);
    mRectR = Rectf(mRectL.getX2(), mPos.y, mRectL.getX2() + c2.x + mTextPadding.x * 2.0f, mPos.y + height + mTextPadding.y * 2.0f);
}

ToggleBtn::~ToggleBtn(){
    mMouseDownCb.disconnect();
}

void ToggleBtn::setColors(ci::ColorA bg, ci::ColorA text)
{
    mBgColor = bg;
    mTextColor = text;
}

void ToggleBtn::onMouseDown(ci::app::MouseEvent event)
{
    vec2 pos = gfx::GFXGlobal::getInstance()->localToGlobal(event.getPos());
    
    if( mRectL.contains(pos) ){
        if(!bLeftActive)
            sOnToggleChange.emit(TOGGLE_SIDE::LEFT);
        bLeftActive = true;
    }else if(mRectR.contains(pos)){
        if(bLeftActive)
            sOnToggleChange.emit(TOGGLE_SIDE::RIGHT);
        bLeftActive = false;
    }
}

void ToggleBtn::draw()
{
    {
        gl::ScopedColor scCol(mBgColor);
        gl::drawStrokedRect(mRectL);
        gl::drawStrokedRect(mRectR);
        
        if(bLeftActive){
            gl::drawSolidRect(mRectL);
            gl::ScopedColor sccc(ColorA(mBgColor.r, mBgColor.g, mBgColor.b, 0.4));
            gl::drawSolidRect(mRectR);
        }else{
            gl::drawSolidRect(mRectR);
            gl::ScopedColor sccc(ColorA(mBgColor.r, mBgColor.g, mBgColor.b, 0.4));
            gl::drawSolidRect(mRectL);
        }
    }
    {
        gl::ScopedColor scC(mTextColor);
        mFont->drawString(mChoice1, mRectL.getLowerLeft() + mTextPadding * vec2(1, -1.7));
        mFont->drawString(mChoice2, mRectR.getLowerLeft() + mTextPadding * vec2(1, -1.7));
    }
}

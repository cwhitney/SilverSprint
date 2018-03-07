#include "DropDown.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

DropDown::DropDown( const ci::Rectf &bounds, ci::gl::TextureFontRef font) {
    mBounds = bounds;
    
    ci::app::WindowRef window = cinder::app::getWindow();
    mMouseDownCb = window->getSignalMouseDown().connect( std::bind(&DropDown::onMouseDown, this, std::placeholders::_1) );
    mMouseUpCb   = window->getSignalMouseUp().connect( std::bind(&DropDown::onMouseUp, this, std::placeholders::_1) );
    
    mFont = font;
}

void DropDown::setOptions(const std::vector<std::string> &opts )
{
    mOptions = opts;
}

void DropDown::onMouseDown( ci::app::MouseEvent event ){
    if(!bVisible){
        return;
    }
    
    vec2 pos = Model::instance().localToGlobal(event.getPos());
    
    if( mBounds.contains(pos) ){
        bOpen = !bOpen;
    }
}

void DropDown::onMouseUp( ci::app::MouseEvent event ){
    if(!bVisible){
        return;
    }
    
    vec2 pos = Model::instance().localToGlobal(event.getPos());
}

void DropDown::draw() {
    gl::ScopedColor scCol(Model::instance().playerColors[0]);
    gl::drawSolidRect( mBounds );
    
    if(mOptions.size() > 0){
        gl::ScopedColor scC(Color::black());
        mFont->drawString(mOptions[0], mBounds + vec2(10, 80));
    }
}



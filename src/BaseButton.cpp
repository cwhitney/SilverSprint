//
//  BaseButton.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 1/6/15.
//
//

#include "BaseButton.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BaseButton::BaseButton() {
    bActive = false;
    bHover = false;
    mBounds = Rectf( 0, 0, 100, 100 );
    
    ci::app::WindowRef win = getWindow();
    win->getSignalMouseMove().connect( std::bind(&BaseButton::onMouseMove, this, std::placeholders::_1));
    win->getSignalMouseUp().connect( std::bind(&BaseButton::onMouseUp, this, std::placeholders::_1));
}

void BaseButton::onMouseUp( cinder::app::MouseEvent event ){
    vec2 pos = gfx::GFXGlobal::getInstance()->localToGlobal( event.getPos() );
    if( mBounds.contains( pos ) ){
        onClick();
    }
}

void BaseButton::onMouseMove( cinder::app::MouseEvent event ){
    vec2 pos = gfx::GFXGlobal::getInstance()->localToGlobal( event.getPos() );

    if( mBounds.contains( pos ) ){
        if( !bHover ){
            onMouseOver();
        }
        bHover = true;
    }else{
        if( bHover ){
            onMouseOut();
        }
        bHover = false;
    }
}
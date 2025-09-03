//
//  BaseButton.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 1/6/15.
//
//

#include "ui/BaseButton.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

BaseButton::BaseButton() {
    bActive = false;
    bHover = false;
    Rectf::set(0,0, 100, 100);
    
    ci::app::WindowRef win = getWindow();
    win->getSignalMouseMove().connect( std::bind(&BaseButton::onMouseMove, this, std::placeholders::_1));
    win->getSignalMouseUp().connect( std::bind(&BaseButton::onMouseUp, this, std::placeholders::_1));
}

void BaseButton::onMouseUp( cinder::app::MouseEvent event ){
    vec2 pos = Model::instance().localToGlobal( event.getPos() );
    if( this->contains( pos ) ){
        onClick();
    }
}

void BaseButton::onMouseMove( cinder::app::MouseEvent event ){
    vec2 pos = Model::instance().localToGlobal( event.getPos() );

    if( this->contains( pos ) ){
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

//
//  BaseButton.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 1/6/15.
//
//

#pragma once

#include "cinder/app/App.h"

class BaseButton {
  public:
    BaseButton();
    
//    virtual void setup();
//    virtual void update();
//    virtual void draw();
    
    void onMouseMove( cinder::app::MouseEvent event );
    void onMouseUp( cinder::app::MouseEvent event );
    
    virtual void onMouseOver() = 0;
    virtual void onMouseOut() = 0;
    virtual void onClick() = 0;
    
  protected:
    ci::Rectf   mBounds;
    bool        bEnabled;
    bool        bHover;
    bool        bActive;
};

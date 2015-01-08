//
//  NumStepper.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 1/7/15.
//
//

#pragma once

#include "cinder/app/AppNative.h"

#include "BaseButton.h"
#include "GFXGlobal.h"

namespace gfx {

class NumStepper : public BaseButton {
  public:
    NumStepper();
    void setup( ci::Vec2f pos, int gap, std::string symbol );
    void draw();
    
    virtual void onMouseOver();
    virtual void onMouseOut();
    virtual void onClick();
    
    ci::signals::signal<void(void)>	signalOnClick;
    
  private:
    GFXGlobal       *mGlobal;
    ci::Color       mBgColor;
    std::string     mTxt;
};
    
}
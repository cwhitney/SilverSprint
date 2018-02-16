//
//  NumStepper.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 1/7/15.
//
//

#pragma once

#include "cinder/app/App.h"

#include "BaseButton.h"
#include "data/GFXGlobal.h"

namespace gfx {

class NumStepper : public BaseButton {
  public:
    NumStepper();
    void setup( ci::vec2 pos, int gap, std::string symbol );
    void draw();
    
    virtual void onMouseOver();
    virtual void onMouseOut();
    virtual void onClick();
    
    ci::signals::Signal<void(void)>	signalOnClick;
    
  private:
    GFXGlobal       *mGlobal;
    ci::Color       mBgColor;
    std::string     mTxt;
};
    
}

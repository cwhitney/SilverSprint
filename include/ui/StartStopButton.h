//
//  StartStopButton.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 1/6/15.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "GFXGlobal.h"
#include "StateManager.h"
#include "BaseButton.h"

namespace gfx {

class StartStopButton : public BaseButton {
  public:
    StartStopButton();
    void update();
    void draw();
    
    virtual void onMouseOver();
    virtual void onMouseOut();
    virtual void onClick();
    
    ci::signals::Signal<void(void)>	signalStartRace, signalStopRace;
    
  protected:
    ci::Color       mBackground;
    StateManager    *mStateManager;
};

}
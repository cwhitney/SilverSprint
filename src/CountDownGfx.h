//
//  CountDownGfx.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 3/17/15.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Timeline.h"

#include "StateManager.h"

namespace gfx {
    
class CountDownGfx {
  public:
    CountDownGfx();
    void draw();
    
  private:
    void onRaceStateChange( RACE_STATE newState );
    
    StateManager        *mStateManager;
    RACE_STATE          mCurRaceState;
    
    std::string         mTxt;
    ci::Anim<float>     timerf;
};

}
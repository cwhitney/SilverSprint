//
//  SettingsView.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/15/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

#include "StateManager.h"
#include "Model.h"
#include "GFXGlobal.h"

namespace gfx {

class SettingsView {
public:
    SettingsView();
    void update();
    void draw();
    
  private:
    void onStateChange(GFX_STATE newState);
    
    StateManager    *mStateManager;
    Model           *mModel;
    GFXGlobal       *mGlobal;
    
    bool            bVisible;
};

}
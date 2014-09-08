//
//  StateManager.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

namespace gfx{

    enum GFX_STATE {
        IDLE = 0,
        STAGING,
        RACING,
        STOPPED
    };

    class StateManager {
    public:
        static StateManager* getInstance();
        
        void changeState( GFX_STATE newState );
        GFX_STATE getCurrentState();
        
        ci::signals::signal<void(GFX_STATE)>	signalOnStateChange;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        StateManager(){};
        StateManager(StateManager const&) : mCurState(GFX_STATE::IDLE) {};
        static StateManager		*mInstance;
        
        GFX_STATE   mCurState;
    };

}
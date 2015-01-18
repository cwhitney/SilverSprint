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

    enum APP_STATE {
        IDLE = 0,
        RACE,
        ROSTER,
        SETTINGS
    };
    
    enum RACE_STATE {
        RACE_STARTING,
        RACE_COUNTDOWN,
        RACE_RUNNING,
        RACE_STOPPED,
        RACE_COMPLETE,
        RACE_FALSE_START
    };

    class StateManager {
    public:
        static StateManager* getInstance();
        
        void changeState( APP_STATE newState );
        void changeRaceState( RACE_STATE newState );
        
        APP_STATE getCurrentState();
        RACE_STATE getCurrentRaceState();
        
        ci::signals::signal<void(APP_STATE)>	signalOnStateChange;
        ci::signals::signal<void(RACE_STATE)>	signalOnRaceStateChange;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        StateManager(){};
        StateManager(StateManager const&) : mCurState(APP_STATE::IDLE) {};
        static StateManager		*mInstance;
        
        APP_STATE   mCurState;
        RACE_STATE  mCurRaceState;
    };

}
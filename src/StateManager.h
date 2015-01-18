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
        RACE = 0,
        ROSTER,
        SETTINGS
    };
    
    enum RACE_STATE {
        RACE_STARTING = 0,
        RACE_COUNTDOWN,
        RACE_RUNNING,
        RACE_STOPPED,
        RACE_COMPLETE,
        RACE_FALSE_START
    };

    class StateManager {
    public:
        static StateManager* getInstance();
        
        void changeAppState( APP_STATE newState, bool forceEvent=false );
        void changeRaceState( RACE_STATE newState, bool forceEvent=false );
        
        APP_STATE getCurrentAppState();
        RACE_STATE getCurrentRaceState();
        
        ci::signals::signal<void(APP_STATE)>	signalOnStateChange;
        ci::signals::signal<void(RACE_STATE)>	signalOnRaceStateChange;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        StateManager() : mCurAppState(APP_STATE::RACE), mCurRaceState(RACE_STATE::RACE_STOPPED) {};
        StateManager(StateManager const&) : mCurAppState(APP_STATE::RACE), mCurRaceState(RACE_STATE::RACE_STOPPED) {};
        static StateManager		*mInstance;
        
        APP_STATE   mCurAppState;
        RACE_STATE  mCurRaceState;
    };

}
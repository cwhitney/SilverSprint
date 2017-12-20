//
//  StateManager.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"

namespace gfx{

    enum APP_STATE {
        RACE = 0,
        ROSTER,
        SETTINGS
    };
    
    enum RACE_STATE {
        RACE_STARTING = 0,
        RACE_COUNTDOWN_3,
        RACE_COUNTDOWN_2,
        RACE_COUNTDOWN_1,
        RACE_COUNTDOWN_GO,
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
        
        std::string getCurrentAppStateString();
        std::string getCurrentRaceStateString();
        
        std::string getAppStateString( APP_STATE as );
        std::string getRaceStateString( RACE_STATE rs );
        
        //! Emits the current state, followed by the previous state
        ci::signals::Signal<void(APP_STATE, APP_STATE)>	signalOnStateChange;
        ci::signals::Signal<void(RACE_STATE)>	signalOnRaceStateChange;
        
        ci::signals::Signal<void(void)>         signalOnRaceFinished;
        ci::signals::Signal<void(int, int, int)>signalRacerFinish;      // (Racer ID, finish time, race ticks)
        
        ci::signals::Signal<void(void)>         signalArduinoConnected, signalArduinoDisconnected;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        StateManager() : mCurAppState(APP_STATE::RACE), mCurRaceState(RACE_STATE::RACE_STOPPED) {};
        StateManager(StateManager const&) : mCurAppState(APP_STATE::RACE), mCurRaceState(RACE_STATE::RACE_STOPPED) {};
        static StateManager		*mInstance;
        
        APP_STATE   mCurAppState;
        RACE_STATE  mCurRaceState;
    };

}

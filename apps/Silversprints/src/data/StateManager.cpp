//
//  StateManager.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "data/StateManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

APP_STATE StateManager::getCurrentAppState() {
    return mCurAppState;
}

RACE_STATE StateManager::getCurrentRaceState() {
    return mCurRaceState;
}

void StateManager::changeRaceState(gfx::RACE_STATE newState, bool forceEvent ){
    if( newState == mCurRaceState && !forceEvent ){
        return;
    }
    mCurRaceState = newState;
    signalOnRaceStateChange.emit( mCurRaceState );
}

void StateManager::changeAppState( APP_STATE newState, bool forceEvent ){
    if( newState == mCurAppState && !forceEvent ){
        return;
    }
    auto lastState = mCurAppState;
    mCurAppState = newState;
    signalOnStateChange.emit(mCurAppState, lastState);
}

std::string StateManager::getCurrentAppStateString() {
    return getAppStateString( mCurAppState );
}

std::string StateManager::getCurrentRaceStateString() {
    return getRaceStateString( mCurRaceState );
}

std::string StateManager::getAppStateString( APP_STATE as ) {
    switch( as ){
        case RACE:
            return "RACE"; break;
        case ROSTER:
            return "ROSTER"; break;
        case SETTINGS:
            return "SETTINGS"; break;
        default:
            return "UNKNOWN"; break;
    }
}

std::string StateManager::getRaceStateString( RACE_STATE rs ) {
    switch( rs ){
        case RACE_STARTING:
            return "RACE_STARTING"; break;
        case RACE_COUNTDOWN_3:
            return "RACE_COUNTDOWN_3"; break;
        case RACE_COUNTDOWN_2:
            return "RACE_COUNTDOWN_2"; break;
        case RACE_COUNTDOWN_1:
            return "RACE_COUNTDOWN_1"; break;
        case RACE_COUNTDOWN_GO:
            return "RACE_COUNTDOWN_GO"; break;
        case RACE_RUNNING:
            return "RACE_RUNNING"; break;
        case RACE_STOPPED:
            return "RACE_STOPPED"; break;
        case RACE_COMPLETE:
            return "RACE_COMPLETE"; break;
        case RACE_FALSE_START:
            return "RACE_FALSE_START"; break;
        default:
            return "UNKNOWN"; break;
    };
}

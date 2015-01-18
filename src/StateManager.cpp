//
//  StateManager.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "StateManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

StateManager* StateManager::mInstance = NULL;
 
StateManager * StateManager::getInstance(){
    if (!mInstance){
        mInstance = new StateManager();
//        mInstance->changeAppState( APP_STATE::RACE );
//        mInstance->changeRaceState( RACE_STATE::RACE_STOPPED );
    }
    return mInstance;
}

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
    signalOnRaceStateChange( mCurRaceState );
}

void StateManager::changeAppState( APP_STATE newState, bool forceEvent ){
    if( newState == mCurAppState && !forceEvent ){
        return;
    }
    mCurAppState = newState;
    signalOnStateChange( mCurAppState );
}
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
    }
    return mInstance;
}

APP_STATE StateManager::getCurrentState() {
    return mCurState;
}

RACE_STATE StateManager::getCurrentRaceState() {
    return mCurRaceState;
}

void StateManager::changeRaceState(gfx::RACE_STATE newState){
    if( newState == mCurRaceState ){
        return;
    }
    mCurRaceState = newState;
    signalOnRaceStateChange( mCurRaceState );
}

void StateManager::changeState( APP_STATE newState ){
    if( newState == mCurState ){
        return;
    }
    mCurState = newState;
    signalOnStateChange( mCurState );
}
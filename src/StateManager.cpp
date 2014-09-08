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

GFX_STATE StateManager::getCurrentState() {
    return mCurState;
}

void StateManager::changeState( GFX_STATE newState ){
    mCurState = newState;
    signalOnStateChange( mCurState );
}
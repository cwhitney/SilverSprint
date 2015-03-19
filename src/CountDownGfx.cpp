//
//  CountDownGfx.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 3/17/15.
//
//

#include "CountDownGfx.h"
#include "GFXGlobal.h"

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

CountDownGfx::CountDownGfx() {
    mStateManager = StateManager::getInstance();
    mStateManager->signalOnRaceStateChange.connect( std::bind(&CountDownGfx::onRaceStateChange, this, std::placeholders::_1) );
    
    mTxt = "";
}

void CountDownGfx::onRaceStateChange( RACE_STATE newState ) {
    
    if( newState == RACE_COUNTDOWN_3 ){
        mTxt = "3";
    }else if( newState == RACE_COUNTDOWN_2 ){
        mTxt = "2";
    }else if( newState == RACE_COUNTDOWN_1 ){
        mTxt = "1";
    }else if( newState == RACE_COUNTDOWN_GO ){
        mTxt = "GO";
        
        timerf = 0.0;
        timeline().apply( &timerf, 0.0f, 1.0f, 1.0f ).finishFn( [&](){ mTxt = ""; } );
    }
    
    else if( newState == RACE_STOPPED  ){
        mTxt = "";
    }
}

void CountDownGfx::draw() {
    if( mTxt != ""){
        gl::color( Color::white() );
        Vec2f txtSize = GFXGlobal::getInstance()->countdownFont->measureString( mTxt );
        
        GFXGlobal::getInstance()->countdownFont->drawString(mTxt, Vec2f(1920 / 2, 1080 / 2) + txtSize * -0.5 );
    }
}
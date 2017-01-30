//
//  StartStopButton.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 1/6/15.
//
//

#include "ui/StartStopButton.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

StartStopButton::StartStopButton()
{
    mBounds = Rectf(1700, 23, 1700 + 174, 23 + 48 );
    mBackground = ci::Color( 22.0/255.0, 146.0/255.0, 84.0/255.0 );
    
    mStateManager = StateManager::getInstance();
}

void StartStopButton::update()
{
    
}

void StartStopButton::onMouseOver()
{
    mBackground = ci::Color( 1, 1, 1 );
}

void StartStopButton::onMouseOut()
{
    if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_RUNNING ){
        mBackground = ci::Color::hex( 0x1c9185 );
    }else{
        mBackground = ci::Color( 22.0/255.0, 146.0/255.0, 84.0/255.0 );
    }
}

void StartStopButton::onClick()
{
    if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_STOPPED ||
       mStateManager->getCurrentRaceState() == RACE_STATE::RACE_COMPLETE ){
        signalStartRace.emit();
    }else{
        signalStopRace.emit();
    }
}

void StartStopButton::draw()
{
    gl::color( mBackground );
    gl::drawSolidRect( mBounds );
    
    gl::color( Color::white() );
    
    if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_STOPPED ||
       mStateManager->getCurrentRaceState() == RACE_STATE::RACE_COMPLETE ){
        gl::color( Color::black() );
        GFXGlobal::getInstance()->texFont->drawString("START", mBounds.getLowerLeft() + vec2(40, -15) );
    }else {
        gl::color( Color::black() );
        GFXGlobal::getInstance()->texFont->drawString("STOP", mBounds.getLowerLeft() + vec2(45, -15) );
    }
}
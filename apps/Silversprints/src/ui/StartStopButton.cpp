//
//  StartStopButton.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 1/6/15.
//
//

#include "ui/StartStopButton.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

StartStopButton::StartStopButton() :
    BaseButton()
{
    BaseButton::set(1700, 23, 1700 + 174, 23 + 48 );
    mBackground = ci::Color( 22.0/255.0, 146.0/255.0, 84.0/255.0 );
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
    if( StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_RUNNING ){
        mBackground = ci::Color::hex( 0x1c9185 );
    }else{
        mBackground = ci::Color( 22.0/255.0, 146.0/255.0, 84.0/255.0 );
    }
}

void StartStopButton::onClick()
{
    if( StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_STOPPED ||
       StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_COMPLETE ){
        signalStartRace.emit();
    }else{
        signalStopRace.emit();
    }
}

void StartStopButton::draw()
{
    gl::color( mBackground );
    gl::drawSolidRect( *this );
    
    gl::color( Color::white() );
    
    if( StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_STOPPED ||
       StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_COMPLETE ){
        gl::color( Color::black() );
		gl::ScopedMatrices scMat;
		//gl::translate((this->getLowerLeft() + vec2(40, -15))*vec2(-1.0));
		//gl::scale( vec2(1.0 / ci::app::getWindowContentScale()) );
		//gl::translate(this->getLowerLeft() + vec2(40, -15));
		//gl::scale(vec2(0.9));
        Model::instance().texFont->drawString("START", this->getLowerLeft() + vec2(40, -15),
			Model::instance().getTfDrawOpts());
    }else {
        gl::color( Color::black() );
        Model::instance().texFont->drawString("STOP", this->getLowerLeft() + vec2(45, -15),
			Model::instance().getTfDrawOpts());
    }
}

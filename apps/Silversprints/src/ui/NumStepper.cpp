//
//  NumStepper.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 1/7/15.
//
//

#include "ui/NumStepper.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

NumStepper::NumStepper() :
    BaseButton()
{

}
                       
void NumStepper::setup( ci::vec2 pos, int gap, string symbol ){
    BaseButton::set( pos.x, pos.y, pos.x + 50 - gap/2, pos.y + 50 - gap/2 );
    
    mTxt = symbol;
    
    mBgColor = Model::instance().playerColors[0];
}

void NumStepper::draw() {
    gl::color( mBgColor );
    gl::drawSolidRect( *this );
    
    ci::Color txtCol = (mBgColor == Color::black()) ? Model::instance().playerColors[0] : Color::black();
    gl::color( txtCol );

    gl::pushMatrices();
        gl::translate( this->getUpperLeft() );
        gl::drawSolidRect( Rectf(10, 22, 40, 26) );
    
        if( mTxt == "+"){
            gl::drawSolidRect( Rectf(22, 10, 26, 40) );
        }
    gl::popMatrices();
}

void NumStepper::onMouseOver(){
    mBgColor = Color::black();
}

void NumStepper::onMouseOut(){
    mBgColor = Model::instance().playerColors[0];
}

void NumStepper::onClick(){
    signalOnClick.emit();
}

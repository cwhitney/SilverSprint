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

NumStepper::NumStepper(){

}
                       
void NumStepper::setup( ci::vec2 pos, int gap, string symbol ){
    mBounds = Rectf( 0, 0, 50 - gap/2, 50 - gap/2 );
    mBounds.offset( pos );
    
    mTxt = symbol;
    
    mBgColor = Model::instance().playerColors[0];
}

void NumStepper::draw() {
    gl::color( mBgColor );
    gl::drawSolidRect( mBounds );
    
    ci::Color txtCol = (mBgColor == Color::black()) ? Model::instance().playerColors[0] : Color::black();
    gl::color( txtCol );

    gl::pushMatrices();
        gl::translate( mBounds.getUpperLeft() );
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

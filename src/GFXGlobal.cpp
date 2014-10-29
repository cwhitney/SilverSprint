//
//  GFXGlobal.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "GFXGlobal.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

GFXGlobal* GFXGlobal::mInstance = NULL;
 
GFXGlobal * GFXGlobal::getInstance(){
    if (!mInstance){
        mInstance = new GFXGlobal();
        
        mInstance->uiFont = ci::Font( loadAsset("fonts/UbuntuMono-R.ttf"), 35 );
        mInstance->texFont = gl::TextureFont::create( mInstance->uiFont );
        mInstance->currentRaceType = RACE_TYPE::RACE_TYPE_TIME;
    }
    return mInstance;
}

void GFXGlobal::setScale( const ci::Rectf &scaledRect ){
    float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    
    mScreenScale = Vec2f(mat[0], mat[0]);
    mScreenOffset = scaledRect.getUpperLeft();
}

ci::Vec2f GFXGlobal::localToGlobal( ci::Vec2f pos ){
    return (Vec2f(pos - mScreenOffset) / mScreenScale);
}

std::string addZ(int n) {
    return (n<10? "0":"") + to_string(n);
}

std::string GFXGlobal::toTimestamp( int millis ){
    //    millis = 755020;
    
    int ms = millis % 1000;
    millis = (millis - ms) / 1000;
    int secs = millis % 60;
    millis = (millis - secs) / 60;
    int mins = millis % 60;
    
    while( ms > 100 ){
        ms = floor(ms / 10);
    }
    
    return addZ(mins) + ":" + addZ(secs) + ":" + addZ(ms);
}
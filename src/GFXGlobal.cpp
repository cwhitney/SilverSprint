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
        
        mInstance->uiFont = ci::Font( "Helvetica", 35 );
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
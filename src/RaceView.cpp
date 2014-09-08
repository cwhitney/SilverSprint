//
//  RaceView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "RaceView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

RaceView::RaceView() : bVisible(false) {
    
}

void RaceView::setup(){
    mBg = gl::Texture::create( loadImage( loadAsset("raceBackground.png") ) );
    mStateManager = StateManager::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&RaceView::onStateChange, this, std::placeholders::_1) );
}

void RaceView::onStateChange(GFX_STATE newState){
    console() << "Raceview :: State change :: " << (newState != GFX_STATE::STAGING) << endl;
    
    if( newState != GFX_STATE::STAGING ){
        bVisible = true;
    }else{
        bVisible = false;
    }
}

void RaceView::update(){
    
}

void RaceView::draw(){
    if( !bVisible ){
        return;
    }
    
    if( mBg ){
        gl::color(1,1,1);
        gl::draw( mBg );
    }
}
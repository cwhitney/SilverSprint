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
//    mBg = gl::Texture::create( loadImage( loadAsset("img/guide_01.png") ) );
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    
    mGlobal = gfx::GFXGlobal::getInstance();
    
    for(int i=0; i<4; i++){
        mNeedles.push_back( gl::Texture::create( loadImage(loadAsset("needle.png")) ) );
        mRaceTexts.push_back( new RaceText( mGlobal->playerColors[i] ) );
    }
    
    mDial = gl::Texture::create( loadImage(loadAsset("img/dial_bg.png")) );
    
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&RaceView::onStateChange, this, std::placeholders::_1) );
}

void RaceView::onStateChange(GFX_STATE newState){
    if( newState == GFX_STATE::IDLE || newState == GFX_STATE::RACING ){
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
    
    if( mStateManager->getCurrentState() == GFX_STATE::RACING || true){
        
        for( int i=0; i<4; i++){
            PlayerData *pd = Model::getInstance()->playerData[i];
            mRaceTexts[i]->draw( pd, Vec2f(0, 390 + 102*i) );
            
//            gl::drawString("RaceTime :: " + to_string(mModel->elapsedRaceTimeMillis), Vec2f(10,10), Color::black() );
        }
        
        gl::draw( mDial, mDial->getSize() * -0.5 + Vec2f(967, 580) );
    }
}



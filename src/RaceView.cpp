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
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    mFinishFlag = gl::Texture::create( loadImage( loadAsset("img/finishFlag.png") ) );
    mProgLine = gl::Texture::create( loadImage( loadAsset("img/progLine.png") ) );
    
    mGlobal = gfx::GFXGlobal::getInstance();
    
    for(int i=0; i<4; i++){
        mRaceTexts.push_back( new RaceText( mGlobal->playerColors[i] ) );
    }
    
    mDial = gl::Texture::create( loadImage(loadAsset("img/dial_bg.png")) );
    
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&RaceView::onStateChange, this, std::placeholders::_1) );
    
    mTimerFont = ci::gl::TextureFont::create( ci::Font(loadAsset("fonts/UbuntuMono-B.ttf"), 50) );
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

    gl::drawSolidRect( Rectf(834,105,834+260,105+185) );    // big white rect
    gl::drawSolidRect( Rectf(60,133,1870,135) );            // white line
    
    if( mStateManager->getCurrentState() == GFX_STATE::RACING || true){
        
        // PLAYER INFO
        for( int i=0; i<mModel->getNumRacers(); i++){
            PlayerData *pd = Model::getInstance()->playerData[i];
            mRaceTexts[i]->draw( pd, Vec2f(0, 390 + 102*i) );
        }
        
        // MAIN TIMER
        gl::color(0,0,0,1);
        
        if( mModel->raceState == RACE_STATE::RACE_RUNNING ){
            int elapsedTime = (getElapsedSeconds()*1000.0) - mModel->startTimeMillis;
            
            mModel->elapsedRaceTimeMillis = elapsedTime;
            
            mTimerFont->drawString( mGlobal->toTimestamp(elapsedTime), Vec2f(867,154) );
        }else if( mModel->raceState == RACE_STATE::RACE_IDLE ){
            mTimerFont->drawString( mGlobal->toTimestamp(0), Vec2f(867,154) );
        }
        
        // DIAL
        gl::color(1,1,1,1);
        gl::draw( mDial, mDial->getSize() * -0.5 + Vec2f(967, 580) );
        
        // LINES
        for( int i=0; i<mModel->getNumRacers(); i++){
            PlayerData *pd = Model::getInstance()->playerData[i];
            gl::pushMatrices();{
                gl::translate( Vec2f(967, 608) );
                gl::rotate( 360.0 * pd->getPercent() );
                gl::translate( Vec2f(mProgLine->getWidth()*-0.5 + 2, -mProgLine->getHeight() - 42 ) );
                gl::color( mGlobal->playerColors[i] );
                gl::draw( mProgLine );
            }gl::popMatrices();
        }
    }
    
    // FLAG
    gl::color(1,1,1,1);
    if( mFinishFlag && mGlobal->currentRaceType == RACE_TYPE::RACE_TYPE_TIME ){
        gl::draw(mFinishFlag, Vec2f(873, 130));
    }
}



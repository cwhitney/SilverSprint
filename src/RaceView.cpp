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
    mModel = Model::getInstance();
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
    
    if( mStateManager->getCurrentState() == GFX_STATE::RACING){
        
        for( int i=0; i<4; i++){
            PlayerData *pd = Model::getInstance()->playerData[i];
            
            gl::pushMatrices();{
                gl::translate(917, 180*i + 30);
                
                if( pd->didFinishRace() ){
                    gl::color( Color::white() );
                }else{
                    gl::color( Color::gray(0.8) );
                }
                gl::drawSolidRect( Rectf(0,0,300,150) );
                
                gl::color( Color::black() );
                gl::drawString(pd->player_name, Vec2f(20,20), Color::black() );
                gl::drawString("Dist: " + to_string( pd->getDistance() ), Vec2f(20,40), Color::black() );
                gl::drawString( to_string(pd->getMph()) + " mph", Vec2f(20,60), Color::black() );
                
            }gl::popMatrices();
            
            gl::drawString("RaceTime :: " + to_string(mModel->elapsedRaceTimeMillis), Vec2f(10,10), Color::black() );
        }
    }
}




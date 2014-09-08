//
//  StagingView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "StagingView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

StagingView::StagingView() : bVisible(false) {
    
}

void StagingView::setup(){
    mBg = gl::Texture::create( loadImage( loadAsset("stagingBackground.png") ) );
    mStateManager = StateManager::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&StagingView::onStateChange, this, std::placeholders::_1) );
}

void StagingView::onStateChange(GFX_STATE newState){
    console() << "Staging view :: state change :: " << (newState == GFX_STATE::STAGING) << endl;
    
    if( newState == GFX_STATE::STAGING ){
        bVisible = true;
    }else{
        bVisible = false;
    }
}

void StagingView::update(){
    
}

void StagingView::start(){
    // reset all values here
}

void StagingView::stop(){
    
}

void StagingView::draw(){
    if( !bVisible ){
        return;
    }
    
    if( mBg ){
        gl::color(1,1,1);
        gl::draw( mBg );
    }
}
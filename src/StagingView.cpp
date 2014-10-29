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
    mModel = Model::getInstance();
    mGlobal = gfx::GFXGlobal::getInstance();
    
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    mStateManager = StateManager::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&StagingView::onStateChange, this, std::placeholders::_1) );
    
    for( int i=0; i<4; i++){
        float yPos = 190 + 183 * i;
        
        CiTextField *tf = new CiTextField("Player "+to_string(i+1), Rectf(574, yPos, 574 + 910, yPos+150), ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 75) );
        tf->mColorStroke = mGlobal->playerColors[i];
        tf->mColorFill = mGlobal->playerColors[i];
        tf->mColorText = Color::black();
        tf->padding = Vec2f(30,30);
        
        tf->bUseScissorTest = false;
        mPlayerNames.push_back(tf);
    }
    
    ci::app::WindowRef win = getWindow();
    win->getSignalKeyDown().connect(std::bind(&StagingView::onKeyDown, this, std::placeholders::_1));
}

void StagingView::onKeyDown( ci::app::KeyEvent event ) {
    
    if( event.getCode() == KeyEvent::KEY_TAB ){
        int i=0;
        for( ; i<mPlayerNames.size(); i++ ){
            if( mPlayerNames[i]->isActive() ){
                mPlayerNames[i]->blur();
                
                if( i == mPlayerNames.size()-1 ){
                    mPlayerNames[0]->focus(true);
                }else{
                    mPlayerNames[i+1]->focus(true);
                }
                break;
            }
        }
        if( i == mPlayerNames.size() ){ // nothing selected. so select the first field
            mPlayerNames[0]->focus(true);
        }
    }
}

void StagingView::onStateChange(GFX_STATE newState){
    if( newState == GFX_STATE::ROSTER ){
        animateIn();
    }else{
        animateOut();
    }
}

void StagingView::animateIn(){
    if( bVisible ){
        return;
    }
    
    bVisible = true;
}

void StagingView::animateOut(){
    if(!bVisible){
        return;
    }
    
    for( int i=0; i<mPlayerNames.size(); i++ ){
        mModel->playerData[i]->player_name = mPlayerNames[i]->getText();
    }
    
    bVisible = false;
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
    
    for(int i=0; i<mPlayerNames.size(); i++){
        mPlayerNames[i]->draw();
    }
    
//    gui->draw();
}
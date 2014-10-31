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
    mCancelBtn = gl::Texture::create( loadImage( loadAsset("img/rosterCancel.png") ) );
    
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
        
        mCancelRects.push_back( Rectf(1498, yPos, 1555, yPos+52) );
    }
    
    tFont = ci::gl::TextureFont::create( ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 75) );
    
    ci::app::WindowRef win = getWindow();
    win->getSignalKeyDown().connect(std::bind(&StagingView::onKeyDown, this, std::placeholders::_1));
    win->getSignalMouseUp().connect(std::bind(&StagingView::onMouseUp, this, std::placeholders::_1));
}

void StagingView::onMouseUp( ci::app::MouseEvent event ) {
    
//    Vec2f pos = mGlobal->localToGlobal( event.getPos() );
//    
//    for( int i=0; i<4; i++){
//        if( mCancelRects[i].contains(pos) ){
//            mPlayerNames[i]->disable();
//            break;
//        }
//    }
    
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
    for( auto it = mPlayerNames.begin(); it!=mPlayerNames.end(); ++it){
        (*it)->visible = true;
    }
}

void StagingView::animateOut(){
    if(!bVisible){
        return;
    }
    
    for( int i=0; i<mPlayerNames.size(); i++ ){
        mModel->playerData[i]->player_name = mPlayerNames[i]->getText();
    }
    
    bVisible = false;
    for( auto it = mPlayerNames.begin(); it!=mPlayerNames.end(); ++it){
        (*it)->visible = false;
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
    
    for(int i=0; i<mModel->getNumRacers(); i++){
        mPlayerNames[i]->draw();
        
//        if( mPlayerNames[i]->isEnabled() ){
//            gl::color( Color::hex(0x717174) );
//            gl::drawSolidRect( mCancelRects[i] );
//            gl::color(1,1,1,1);
//            gl::draw( mCancelBtn, mCancelRects[i].getUpperLeft() + Vec2f(14, 12) );
//        }
        
        Vec2f p = mPlayerNames[i]->getBounds().getUpperLeft() + Vec2f(-102, 73);
        if( !mPlayerNames[i]->isActive() ){
            gl::color( mPlayerNames[i]->mColorStroke );
            gl::drawSolidCircle(p, 62);
            gl::color( Color::hex(0x16151b) );
        }else {
            gl::lineWidth(4.0);
            gl::color( mPlayerNames[i]->mColorStroke );
            gl::drawStrokedCircle(p, 62);
            gl::lineWidth(1.0);
        }
        
        tFont->drawString( to_string(i+1), p + Vec2f(-19, 20) );
    }
    
//    gui->draw();
}
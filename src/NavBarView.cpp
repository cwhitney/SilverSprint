//
//  NavBarView.cpp
//  GoldsprintsFX
//
//  Created by Grampa Charlie on 10/24/14.
//
//

#include "NavBarView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

NavBarView::NavBarView(){
    activeIcon = 0;
}

void NavBarView::setup(){
    mFlagTex = gl::Texture::create( loadImage( loadAsset("img/icons/icon_flag.png") ) );
    mRosterTex = gl::Texture::create( loadImage( loadAsset("img/icons/icon_roster.png") ) );
    mSettingsTex = gl::Texture::create( loadImage( loadAsset("img/icons/icon_settings.png") ) );
    
    mFlagRect = mFlagTex->getBounds();
    mFlagRect.offset( Vec2f(47, 28) );
    
    mRosterRect = mRosterTex->getBounds();
    mRosterRect.offset( Vec2f(134, 28) );
    
    mSettingsRect = mSettingsTex->getBounds();
    mSettingsRect.offset( Vec2f(237, 28) );
    
    mIconList.push_back( NavIcon( mFlagTex, mFlagRect, true, GFX_STATE::RACING ) );
    mIconList.push_back( NavIcon( mRosterTex, mRosterRect, false, GFX_STATE::ROSTER ) );
    mIconList.push_back( NavIcon( mSettingsTex, mSettingsRect, false, GFX_STATE::SETTINGS ) );
    
    ci::app::WindowRef win = getWindow();
    win->getSignalMouseUp().connect(std::bind(&NavBarView::mouseUp, this, std::placeholders::_1));
}

void NavBarView::mouseUp(cinder::app::MouseEvent event){
    Vec2f pos = GFXGlobal::getInstance()->localToGlobal(event.getPos());
    
    int prev = activeIcon;
    
    for( int i=0; i<mIconList.size(); i++){
        if( mIconList[i].rect.contains(pos) ){
            activeIcon = i;
        }
        mIconList[i].bActive = false;
    }
    
    mIconList[activeIcon].bActive = true;
    
    if( activeIcon != prev){
        StateManager::getInstance()->changeState( mIconList[activeIcon].state );
    }
    
}

void NavBarView::draw() {
    
    for( int i=0; i<mIconList.size(); i++){
        if( mIconList[i].bActive ){
            gl::color( ColorA(1,1,1,1) );
        }else{
            gl::color( ColorA(1,1,1,0.4) );
        }
        
        gl::draw( mIconList[i].tex, mIconList[i].rect);
    }
    
    
    
}
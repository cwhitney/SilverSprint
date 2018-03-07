//
//  NavBarView.cpp
//  SilverSprints
//
//  Created by Grampa Charlie on 10/24/14.
//
//

#include "views/NavBarView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

NavBarView::NavBarView()
{
    activeIcon = 0;
}

void NavBarView::setup()
{
    mFlagTex = gl::Texture::create( loadImage( loadAsset("img/icons/icon_flag.png") ) );
    mRosterTex = gl::Texture::create( loadImage( loadAsset("img/icons/icon_roster.png") ) );
    mSettingsTex = gl::Texture::create( loadImage( loadAsset("img/icons/icon_settings.png") ) );
    
    mFlagRect = mFlagTex->getBounds();
    mFlagRect.offset( vec2(47, 28) );
    
    mRosterRect = mRosterTex->getBounds();
    mRosterRect.offset( vec2(134, 28) );
    
    mSettingsRect = mSettingsTex->getBounds();
    mSettingsRect.offset( vec2(237, 28) );
    
    mIconList.push_back( NavIcon( mFlagTex, mFlagRect, true, APP_STATE::RACE ) );
    mIconList.push_back( NavIcon( mRosterTex, mRosterRect, false, APP_STATE::ROSTER ) );
    mIconList.push_back( NavIcon( mSettingsTex, mSettingsRect, false, APP_STATE::SETTINGS ) );
    
    ci::app::WindowRef win = getWindow();
    win->getSignalMouseUp().connect(std::bind(&NavBarView::mouseUp, this, std::placeholders::_1));
    
    StateManager::instance().signalOnStateChange.connect( [&](APP_STATE newState, APP_STATE lastState){
        for( int i=0; i<mIconList.size(); i++){
            mIconList[i].bActive = false;
        }
        if( newState == APP_STATE::RACE )      mIconList[0].bActive = true;
        if( newState == APP_STATE::ROSTER )    mIconList[1].bActive = true;
        if( newState == APP_STATE::SETTINGS )  mIconList[2].bActive = true;
    });
}

void NavBarView::mouseUp(cinder::app::MouseEvent event){
    vec2 pos = Model::instance().localToGlobal(event.getPos());
    
    int prev = activeIcon;
    
    for( int i=0; i<mIconList.size(); i++){
        if( mIconList[i].rect.contains(pos) ){
            activeIcon = i;
        }
        mIconList[i].bActive = false;
    }
    
    mIconList[activeIcon].bActive = true;
    
    if( activeIcon != prev){
        StateManager::instance().changeAppState( mIconList[activeIcon].state );
    }
    
}

void NavBarView::draw()
{
    for( int i=0; i<mIconList.size(); i++){
        if( mIconList[i].bActive ){
            gl::color( ColorA(1,1,1,1) );
        }else{
            gl::color( ColorA(1,1,1,0.4) );
        }
        
        gl::draw( mIconList[i].tex, mIconList[i].rect);
    }
}

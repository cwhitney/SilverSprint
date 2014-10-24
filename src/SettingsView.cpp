//
//  SettingsView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/15/14.
//
//

#include "SettingsView.h"

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

SettingsView::SettingsView() : bVisible(false) {
    mGlobal = gfx::GFXGlobal::getInstance();
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&SettingsView::onStateChange, this, std::placeholders::_1) );
}

void SettingsView::onStateChange(GFX_STATE newState) {
    if( newState == GFX_STATE::SETTINGS ){
        bVisible = true;
    }else{
        bVisible = false;
    }
}

void SettingsView::update(){
    
}

void SettingsView::draw(){
    if( bVisible ){
        gl::drawString("SETTINGSSSS", Vec2f(20,20) );
    }
}
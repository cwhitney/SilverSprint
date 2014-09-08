//
//  GFXMain.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "GFXMain.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

GFXMain::GFXMain(){
    
}

void GFXMain::setup(){
    // DATA
    mModel = Model::getInstance();
    
    mSerialReader = new SerialReader();
    mSerialReader->setup();
    
    mStateManager = StateManager::getInstance();
    
    // VIEWS
    mRaceView = new RaceView();
    mStagingView = new StagingView();
    mRaceView->setup();
    mStagingView->setup();
    
    ci::app::WindowRef win = getWindow();
    win->getSignalKeyDown().connect(std::bind(&GFXMain::onKeyDown, this, std::placeholders::_1));
}

void GFXMain::onKeyDown(KeyEvent event){
    if( event.getChar() == 'p'){
        mSerialReader->pingSensor();
    }else if( event.getChar() == 'c'){
        mSerialReader->setCountdown(10);
    }else if( event.getChar() == 'h'){
        mSerialReader->getHardwareVersion();
        mSerialReader->getFirmwareVersion();
        mSerialReader->getProtoculVersion();
    }
    
    else if(event.getChar() == 'g'){
        mStateManager->changeState( GFX_STATE::RACING );
        mSerialReader->startRace();
    }else if(event.getChar() == 's'){
        mStateManager->changeState( GFX_STATE::IDLE );
        mSerialReader->stopRace();
    }
}

void GFXMain::update() {
    mSerialReader->update();
    
    mRaceView->update();
    mStagingView->update();
}

void GFXMain::draw(){
    mRaceView->draw();
    mStagingView->draw();
}
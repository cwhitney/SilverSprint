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

GFXMain::~GFXMain(){
    console() << "GFX main shutting down" << endl;
    delete mSerialReader;
    
    delete mNav;
    delete mRaceView;
    delete mStagingView;
    delete mSettingsView;
}

void GFXMain::setup(){
    // DATA
    mModel = Model::getInstance();
    mGlobal = GFXGlobal::getInstance();
    
    mSerialReader = new SerialReader();
    mSerialReader->setup();
    
    mStateManager = StateManager::getInstance();
    
    // VIEWS
    mNav = new NavBarView();
    mRaceView = new RaceView();
    mStagingView = new StagingView();
    mSettingsView = new SettingsView();
    
    mNav->setup();
    mRaceView->setup();
    mStagingView->setup();
    
    ci::app::WindowRef win = getWindow();
    win->getSignalKeyDown().connect(std::bind(&GFXMain::onKeyDown, this, std::placeholders::_1));
    
    mStateManager->changeState( GFX_STATE::IDLE );
}

void GFXMain::onKeyDown(KeyEvent event){
    if( !event.isAccelDown() ){
        return;
    }
    
    if( event.getChar() == 'p'){
        mSerialReader->pingSensor();
    }else if( event.getChar() == 'c'){
        mSerialReader->setCountdown(4);
    }
    
    else if(event.getChar() == 'g'){
        resetPlayerData();
        mSerialReader->setRaceLengthTicks( mModel->getRaceLengthTicks() );
        
        mStateManager->changeState( GFX_STATE::RACING );
        mSerialReader->startRace();
    }else if(event.getChar() == 's'){
        mStateManager->changeState( GFX_STATE::IDLE );
        mSerialReader->stopRace();
    }
    
    else if( event.getChar() == 'r'){
        mSerialReader->resetHardwareToDefault();
        resetPlayerData();
    }else if( event.getChar() == 'm'){
        mSerialReader->setMockMode();
    }
    
    else if( event.getChar() == 'l'){
        mSerialReader->setRaceLengthTicks( mModel->getRaceLengthTicks() );
    }
}

void GFXMain::resetPlayerData(){
    for( int i=0; i<mModel->playerData.size(); i++){
        mModel->playerData[i]->reset();
        mModel->playerData[i]->setRollerDiameter( fromString<float>(mModel->rollerDiameterMm) );
    }
}

void GFXMain::update() {
    mSerialReader->update();
    
    mRaceView->update();
    mStagingView->update();
    mSettingsView->update();
}

void GFXMain::draw( const Rectf &drawRect ){
    mGlobal->setScale(drawRect);
    
    mRaceView->draw();
    mStagingView->draw();
    mSettingsView->draw();
    
    mNav->draw();
}
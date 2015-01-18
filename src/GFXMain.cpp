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
    
    mSerialReader->signalOnRaceFinished.connect( bind( &GFXMain::onRaceFinished, this ) );
    
    mStateManager->signalOnStateChange.connect( bind( &GFXMain::onStateChaged, this ) );
    mStateManager->signalOnRaceStateChange.connect( bind( &GFXMain::onRaceStateChanged, this ) );
    
    mStateManager->changeAppState( APP_STATE::RACE, true );
    mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED, true );
}

void GFXMain::onRaceFinished() {
    console() << "RACE FINSIHED" << endl;
    mSerialReader->stopRace();
    mStateManager->changeRaceState( RACE_STATE::RACE_COMPLETE );
}

void GFXMain::onStateChaged() {
    
}

void GFXMain::onRaceStateChanged(){
    
    if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_STARTING ){
        mSerialReader->setRaceLengthTicks( mModel->getRaceLengthTicks() );
        mSerialReader->startRace();
    }
    else if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_STOPPED ){
        mSerialReader->stopRace();
        mModel->resetPlayers();
        mModel->elapsedRaceTimeMillis = 0.0;
    }
}

void GFXMain::onKeyDown(KeyEvent event){
    if( !event.isAccelDown() ){
        return;
    }
    else if( event.getChar() == 'r'){
        mRaceView->reloadShader();
    }
    else if( event.getChar() == 'm'){
        mSerialReader->setMockMode();
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
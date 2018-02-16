//
//  GFXMain.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "app/GFXMain.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

std::string msToTimeStr(const size_t &millis){
    int ms = millis % 1000;
    int seconds = int(millis / 1000) % 60 ;
    int minutes = int(millis / (1000*60));
    //int hours   = int(millis / (1000*60*60)) % 24;
    
    stringstream ss;
    ss  << setfill('0') << setw(2) << minutes << ":"
    << setfill('0') << setw(2) << seconds << ":"
    << setfill('0') << setw(3) << ms;

    return ss.str();
}

// -------------------------------------
GFXMain::GFXMain(){
    
}

GFXMain::~GFXMain(){
    CI_LOG_I("GFX main shutting down");
}

void GFXMain::setup(){
    // INIT --------------------------------------------------------------
    mGlobal = GFXGlobal::getInstance();
    
    mSerialReader = SerialReaderRef( new SerialReader() );
    mSerialReader->setup();
    
    mStateManager = StateManager::getInstance();
    
    CsvLogger::instance().setHeaders({"Timestamp", "Event", "Racer 1", "Racer 2", "Racer 3", "Racer 4"});
    
    // VIEWS --------------------------------------------------------------
    mNav			= std::make_shared<NavBarView>();
    mRaceView		= std::make_shared<RaceView>();
    mRosterView		= std::make_shared<RosterView>();
    mSettingsView	= std::make_shared<SettingsView>();
    
    mNav->setup();
    mRaceView->setup();
    mRosterView->setup();
    
    // EVENTS --------------------------------------------------------------
    getWindow()->getSignalKeyDown().connect(std::bind(&GFXMain::onKeyDown, this, std::placeholders::_1));
    
    mStateManager->signalOnRaceFinished.connect( bind( &GFXMain::onRaceFinished, this ) );
    mStateManager->signalOnStateChange.connect( bind( &GFXMain::onAppStateChaged, this, std::placeholders::_1 ) );
    mStateManager->signalOnRaceStateChange.connect( bind( &GFXMain::onRaceStateChanged, this, std::placeholders::_1 ) );
    mStateManager->signalRacerFinish.connect( [&](int _id, int _finishMillis, int _finishTicks){
        Model::instance().playerData[_id]->setFinished(_finishMillis, _finishTicks);
    });
        
    // START --------------------------------------------------------------
    mStateManager->changeAppState( APP_STATE::RACE, true );
    mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED, true );
    
    msToTimeStr(13660001);
}

void GFXMain::reloadShaders()
{
    mRaceView->reloadShader();
}

void GFXMain::onRaceFinished() {
    console() << "GFXMAIN :: RACE FINSIHED" << endl;
    mSerialReader->stopRace();
    mStateManager->changeRaceState( RACE_STATE::RACE_COMPLETE );
    
    if(Model::instance().getRaceLogging()){
        // If it's a distance race, log the times
        if(GFXGlobal::getInstance()->currentRaceType == gfx::RACE_TYPE::RACE_TYPE_DISTANCE){
            CsvLogger::instance().log(CsvLogger::RACE_FINISH,
                                      msToTimeStr(Model::instance().playerData[0]->finishTimeMillis),
                                      msToTimeStr(Model::instance().playerData[1]->finishTimeMillis),
                                      msToTimeStr(Model::instance().playerData[2]->finishTimeMillis),
                                      msToTimeStr(Model::instance().playerData[3]->finishTimeMillis));
        }
        // If it's a time race, log the distance
        else {
            if(Model::instance().getUsesKph()){
                CsvLogger::instance().log(CsvLogger::RACE_FINISH,
                                          Model::instance().playerData[0]->getDistanceMeters(),
                                          Model::instance().playerData[1]->getDistanceMeters(),
                                          Model::instance().playerData[2]->getDistanceMeters(),
                                          Model::instance().playerData[3]->getDistanceMeters());
            }else{
                CsvLogger::instance().log(CsvLogger::RACE_FINISH,
                                          Model::instance().playerData[0]->getDistanceFeet(),
                                          Model::instance().playerData[1]->getDistanceFeet(),
                                          Model::instance().playerData[2]->getDistanceFeet(),
                                          Model::instance().playerData[3]->getDistanceFeet());
            }
        }
        
        CsvLogger::instance().write();
    }
}

void GFXMain::onAppStateChaged( APP_STATE as ) {
    
}

void GFXMain::onRaceStateChanged( RACE_STATE rc ){
    
    if( rc == RACE_STATE::RACE_STARTING ){
        if(GFXGlobal::getInstance()->currentRaceType == RACE_TYPE_DISTANCE ){
            mSerialReader->setRaceType(RACE_TYPE_DISTANCE);
            mSerialReader->setRaceLengthTicks( Model::instance().getRaceLengthTicks() );
        }else{
            mSerialReader->setRaceType(RACE_TYPE_TIME);
            mSerialReader->setRaceDuration( Model::instance().getRaceTimeSeconds() );
        }
        
		Model::instance().resetPlayers();
        mSerialReader->startRace();
    }
    
    else if( rc == RACE_STATE::RACE_STOPPED ){
        mSerialReader->stopRace();
     //   Model::instance().resetPlayers();
        Model::instance().elapsedRaceTimeMillis = 0.0;
    }
}

void GFXMain::onKeyDown(KeyEvent event)
{
    if(event.getCode() == KeyEvent::KEY_SPACE && StateManager::getInstance()->getCurrentAppState() == gfx::APP_STATE::RACE){
        if(StateManager::getInstance()->getCurrentRaceState() == gfx::RACE_STATE::RACE_STOPPED){
            mStateManager->changeRaceState( RACE_STATE::RACE_STARTING );
        }else{
            mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED );
        }
    }
    
    if( !event.isAccelDown() && !event.isControlDown() ){
        return;
    }
    else if( event.getChar() == 'm'){
        mSerialReader->setMockMode();
    }
}

void GFXMain::resetPlayerData(){
    for( int i=0; i<Model::instance().playerData.size(); i++){
        Model::instance().playerData[i]->reset();
        Model::instance().playerData[i]->setRollerDiameter(Model::instance().getRollerDiameterMm());
    }
}

void GFXMain::update()
{
    double start_t, end_t;
    start_t = getElapsedSeconds();
    mSerialReader->update();
    end_t = getElapsedSeconds();
//    console() << "SerialReader update time :: " << start_t << " -- " << (end_t - start_t) << endl;
    
    mRaceView->update();
    mRosterView->update();
    mSettingsView->update();
}

void GFXMain::draw( const Rectf &drawRect ){
//    mGlobal->setScale(drawRect);
    
    mRaceView->draw();
    mRosterView->draw();
    mSettingsView->draw();
    
    mNav->draw();
}

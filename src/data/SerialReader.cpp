//
//  SerialReader.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "SerialReader.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

SerialReader::SerialReader() :
    BAUD_RATE(115200),
    bSerialConnected(false),
    bLastConnection(false),
    mStringBuffer(""),
    mHardwareVersion(""),
    mProtoculVersion(""),
    mFirmwareVersion(""),
    bMockEnabled(false),
    mSerial(NULL),
    bThreadShouldQuit(false),
    mReceiveBuffer(100),
    mSendBuffer(100)
{
 
}

SerialReader::~SerialReader()
{
    stopRace();
    bThreadShouldQuit = true;
    mSerialThread->join();
}

void SerialReader::setup()
{
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    
    mSerialThread = std::shared_ptr<std::thread>( new std::thread( std::bind(&SerialReader::updateSerialThread, this) ) );
}

void SerialReader::update()
{
    // if we connected since the last update, notify
    if( bSerialConnected && !bLastConnection){
        onConnect();
    }else if( !bSerialConnected && bLastConnection ){
        onDisconnect();
    }
    bLastConnection = bSerialConnected;
    
    // lock and parse buffered data
    std::lock_guard<std::mutex> guard(mSerialMutex);
    parseFromBuffer();
}

void SerialReader::updateSerialThread()
{
    ThreadSetup threadSetup;
    
    while( !bThreadShouldQuit ){
        std::lock_guard<std::mutex> guard(mSerialMutex);
        
        if(!bSerialConnected ){ // we aren't connected try to connect
            try {
                Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial", true);
                if( dev.getName() == ""){
                    dev = Serial::findDeviceByNameContains("tty.usbmodem", true);
                }
                mSerial = Serial::create( dev, BAUD_RATE );
                bSerialConnected = true;
                mSerial->flush();
                
                for( int i=0; i<mSendBuffer.getSize(); i++){        // clear send buffer
                    std::string tmp;
                    mSendBuffer.popBack(&tmp);
                }
                stopRace();
                
                for( int i=0; i<mReceiveBuffer.getSize(); i++){    // clear receive buffer
                    std::vector<std::string> tmp;
                    mReceiveBuffer.popBack(&tmp);
                }
                
                CI_LOG_I("OpenSprints hardware found successfully. :: ") << dev.getName();
            }
            catch( ... ) {
                bSerialConnected = false;
            }
        }else{
            // make sure we're still connected
            Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial", true);
            if( dev.getName() == ""){
                dev = Serial::findDeviceByNameContains("tty.usbmodem", true);
            }
            if( dev.getName() == ""){
                bSerialConnected = false;
            }else{  // we are connected ------------------------
                // send to arduino
                for(int i=0; i<mSendBuffer.getSize(); i++){
                    std::string msgToSend;
                    mSendBuffer.popBack( &msgToSend );
                    mSerial->writeString( msgToSend );
                }
                
                // receive from arduino
                readSerial();
            }
        }
    }
}

void SerialReader::readSerial()
{
    uint bytesAvailable = mSerial->getNumBytesAvailable();
    uint charsAvailable = floor(bytesAvailable / sizeof(char));
    
    try {
        for(int i=0; i<charsAvailable; i++){
            unsigned char c = mSerial->readByte();
            mStringBuffer += c;
        }
    }catch(...){
        bSerialConnected = false;
    }
    
    int index = mStringBuffer.find("\r\n");
    if(index != string::npos){
        std::string cmd = mStringBuffer.substr(0, index);
        mStringBuffer.replace(0, index+2, "");      // The newline is 2 bytes (\r\n) so add two to remove it
        
        parseCommandToBuffer( cmd );
    }
}

void SerialReader::parseCommandToBuffer( std::string command )
{
    std::vector<std::string> strs;
    boost::split(strs, command, boost::is_any_of(":"));
    
    std::vector<std::string> tmpBuffer;
    tmpBuffer.push_back( strs[0] );
    
    std::string args = "";
    if( strs.size() > 1){
        tmpBuffer.push_back(strs[1]);
    }
    
    mReceiveBuffer.tryPushFront(tmpBuffer);
}

// Parse commands in the main thread that we grabbed during the updateSerial thread
void SerialReader::parseFromBuffer()
{
    int numCmds = mReceiveBuffer.getSize();
    
    for( int i=0; i<numCmds; i++) {
        std::vector<std::string> cmdArgs;
        mReceiveBuffer.popBack( &cmdArgs );
        
        std::string cmd = cmdArgs[0];
        std::string args = (cmdArgs.size() > 1) ? cmdArgs[1] : "";
        
        if( cmd == ""){
            return;
        }
        
        // ------------------------------------------------------------------------------
        // RACE FINISH
        if(cmd == "0F"){
            CI_LOG_D("RACER 1 FINISHED " + args);
            mStateManager->signalRacerFinish.emit(0, fromString<int>(args));
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        else if( cmd == "1F"){
            CI_LOG_D("RACER 2 FINISHED " +args);
            mStateManager->signalRacerFinish.emit(1, fromString<int>(args));
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        else if( cmd == "2F"){
            CI_LOG_D("RACER 3 FINISHED " + args);
            mStateManager->signalRacerFinish.emit(2, fromString<int>(args));
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        else if( cmd == "3F"){
            CI_LOG_D("RACER 4 FINISHED " + args);
            mStateManager->signalRacerFinish.emit(3, fromString<int>(args));
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        
        // ------------------------------------------------------------------------------
        // RACE PROGRESS
        else if(cmd == "R"){
            std::vector<std::string> rdata;
            boost::split(rdata, args, boost::is_any_of(","));
            
            int raceMillis = fromString<int>( rdata[4] );
                                             
            float dt = raceMillis - mModel->elapsedRaceTimeMillis;
            
            mModel->playerData[0]->updateRaceTicks( fromString<int>(rdata[0]), dt );
            mModel->playerData[1]->updateRaceTicks( fromString<int>(rdata[1]), dt );
            mModel->playerData[2]->updateRaceTicks( fromString<int>(rdata[2]), dt );
            mModel->playerData[3]->updateRaceTicks( fromString<int>(rdata[3]), dt );
            mModel->elapsedRaceTimeMillis = raceMillis;
            mModel->startTimeMillis = ci::app::getElapsedSeconds() * 1000.0 - mModel->elapsedRaceTimeMillis;
        }
        
        // ------------------------------------------------------------------------------
        // SETTINGS
        else if( cmd == "CD"){
            CI_LOG_D("SerialReader :: Countdown :: " + args);
            
            if( args == "3" ){
                mStateManager->changeRaceState( RACE_STATE::RACE_COUNTDOWN_3 );
            }else if( args == "2"){
                mStateManager->changeRaceState( RACE_STATE::RACE_COUNTDOWN_2 );
            }else if( args == "1"){
                mStateManager->changeRaceState( RACE_STATE::RACE_COUNTDOWN_1 );
            }else if( args == "0" ){
                mModel->startTimeMillis = ci::app::getElapsedSeconds() * 1000.0;
                
                mStateManager->changeRaceState( RACE_STATE::RACE_COUNTDOWN_GO );
                mStateManager->changeRaceState( RACE_STATE::RACE_RUNNING );
            }
        }
        else if( cmd == "FS"){
            CI_LOG_D("SerialReader :: False start. Racer: " + args);    // 0 based
//            mStateManager->changeRaceState( RACE_STATE::RACE_FALSE_START );
//            mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED );
        }
        else if( cmd == "L"){   // After sending a race length, it will send this confirmation
            CI_LOG_D("SerialReader :: Race Length " + args);
        }
        else if( cmd == "M"){   // Mock mode confirmation
            CI_LOG_D("SerialReader :: Mock mode turned " + args);
        }
        else if( cmd == "V"){   // version
            mFirmwareVersion = args;
        }
        
        else{
            CI_LOG_D("SerialReader :: Unrecognized command :: '" + cmd);
            if(args != ""){
                CI_LOG_D(" with arg :: '"+ args+ "'");
            }
        }
    }
}

void SerialReader::onConnect()
{
    CI_LOG_I("OpenSprints hardware connected.");
    mModel->bHardwareConnected = bSerialConnected;
}

void SerialReader::onDisconnect()
{
    CI_LOG_I("OpenSprints hardware disconnected.");
    mModel->bHardwareConnected = bSerialConnected;
}

void SerialReader::startRace(){
    sendSerialMessage("g");
}

void SerialReader::stopRace(){
    sendSerialMessage("s");
}

void SerialReader::setRaceDuration(int numSeconds){
    sendSerialMessage("t:" + to_string(numSeconds) );
}

void SerialReader::setRaceLengthTicks( int numTicks ){
    sendSerialMessage( "l"+to_string(numTicks) );
}

void SerialReader::setMockMode( bool enabled ){
    bMockEnabled = enabled;
    sendSerialMessage("m");
}

void SerialReader::sendSerialMessage( std::string msg )
{
    std::string toSend = msg + "\n";
    mSendBuffer.pushFront( toSend );
}

bool SerialReader::isRaceFinished()
{
    for( int i=0; i<mModel->getNumRacers(); i++){
        if( !mModel->playerData[i]->isFinished() ){
            return false;
        }
    }
    
    return true;
}




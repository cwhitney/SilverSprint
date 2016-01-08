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
//    BAUD_RATE(9600),
    bSerialConnected(false),
    bLastConnection(false),
    mStringBuffer(""),
    mHardwareVersion(""),
    mProtoculVersion(""),
    mFirmwareVersion(""),
    bMockEnabled(false),
    mSerial(NULL),
    bThreadShouldQuit(false),
    mThreadAlive(0),
    mBuffer(100)
//    mKeepAliveTimer(0)
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
    
    mSerialThread = std::shared_ptr<std::thread>( new std::thread( std::bind(&SerialReader::updateSerial, this) ) );
//    mSerialThread->detach();
    
//    if( attemptHardwareConnection() ){
//        onConnect();
//    }
}

void SerialReader::updateSerial()
{
    ThreadSetup threadSetup;
    
    while( !bThreadShouldQuit ){
        std::lock_guard<std::mutex> guard(mSerialMutex);
        
        ++mThreadAlive;
        
        if(!bSerialConnected ){ // we aren't connected try to connect
            try {
                Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial", true);
                if( dev.getName() == ""){
                    dev = Serial::findDeviceByNameContains("tty.usbmodem", true);
                }
                mSerial = Serial::create( dev, BAUD_RATE );
                bSerialConnected = true;
                mSerial->flush();
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
            }else{
                // we are connected
                
                // send to arduino
                // ...
                
                
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
    
    //    if( mStringBuffer.length() ){
    //        console() << mStringBuffer << endl;
    //    }
    //    mStringBuffer = "";
    
    int index = mStringBuffer.find("\r\n");
    if(index != string::npos){
        std::string cmd = mStringBuffer.substr(0, index);
        mStringBuffer.replace(0, index+2, "");      // The newline is 2 bytes (\r\n) so add two to remove it
        
        parseCommandToBuffer( cmd );
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

void SerialReader::update()
{
    // if we connected since the last update, notify
    if( bSerialConnected && !bLastConnection){
        onConnect();
    }else if( !bSerialConnected && bLastConnection ){
        onDisconnect();
    }
    bLastConnection = bSerialConnected;
    
    std::lock_guard<std::mutex> guard(mSerialMutex);
    
    parseFromBuffer();
    
//    console() << mThreadAlive << endl;
}

/*
bool SerialReader::keepAlive()
{
    if( !bSerialConnected ){
        if( attemptHardwareConnection() ){
            onConnect();
            return true;
        }
    }
    
    return false;
}
*/
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

void SerialReader::sendSerialMessage( std::string msg ){
    if( bSerialConnected ){
        if( mSerial ){
            try{
                CI_LOG_I("Sending message :: ") << msg;
                mSerial->writeString(msg + "\n" );
            }catch(...){
                onDisconnect();
            }
        }else{
            onDisconnect();
        }
        
    }else{
        CI_LOG_W("Hardware not connected");
    }
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
    
    mBuffer.tryPushFront(tmpBuffer);
}


// Parse commands in the main thread that we grabbed during the updateSerial thread
void SerialReader::parseFromBuffer()
{
    int numCmds = mBuffer.getSize();
    
    if( numCmds != 0)  CI_LOG_I("Parsing ") << numCmds << " commands";
    
    for( int i=0; i<numCmds; i++) {
        std::vector<std::string> cmdArgs;
        mBuffer.popBack( &cmdArgs );
        
        std::string cmd = cmdArgs[0];
        std::string args = (cmdArgs.size() > 1) ? cmdArgs[1] : "";
        
        if( cmd == ""){
            return;
        }
        
        // ------------------------------------------------------------------------------
        // RACE FINISH
        if( cmd == "0F"){
            console() << "RACER 1 FINISHED " << args << endl;
            mModel->playerData[0]->setFinished( fromString<int>(args) );
            if( isRaceFinished() ){ signalOnRaceFinished.emit(); }
        }
        else if( cmd == "1F"){
            console() << "RACER 2 FINISHED " << args << endl;
            mModel->playerData[1]->setFinished( fromString<int>(args) );
            if( isRaceFinished() ){ signalOnRaceFinished.emit(); }
        }
        else if( cmd == "2F"){
            console() << "RACER 3 FINISHED " << args << endl;
            mModel->playerData[2]->setFinished( fromString<int>(args) );
            if( isRaceFinished() ){ signalOnRaceFinished.emit(); }
        }
        else if( cmd == "3F"){
            console() << "RACER 4 FINISHED " << args << endl;
            mModel->playerData[3]->setFinished( fromString<int>(args) );
            if( isRaceFinished() ){ signalOnRaceFinished.emit(); }
        }
        // ------------------------------------------------------------------------------
        // RACE PROGRESS
        else if( cmd == "R"){
            std::vector<std::string> rdata;
            boost::split(rdata, args, boost::is_any_of(","));
            
            mModel->playerData[0]->curRaceTicks = fromString<int>( rdata[0] );
            mModel->playerData[1]->curRaceTicks = fromString<int>( rdata[1] );
            mModel->playerData[2]->curRaceTicks = fromString<int>( rdata[2] );
            mModel->playerData[3]->curRaceTicks = fromString<int>( rdata[3] );
            mModel->elapsedRaceTimeMillis = fromString<int>( rdata[4] );
            mModel->startTimeMillis = ci::app::getElapsedSeconds() * 1000.0 - mModel->elapsedRaceTimeMillis;
        }
        
        // ------------------------------------------------------------------------------
        // SETTINGS
        else if( cmd == "A"){
            console() << "SerialReader :: Got ping :: " << args << endl;
        }
        else if( cmd == "CD"){
            console() << "SerialReader :: Countdown :: " << args << endl;
            
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
            console() << "SerialReader :: False start. Racer: " << args << endl;    // 0 based
            mStateManager->changeRaceState( RACE_STATE::RACE_FALSE_START );
            mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED );
        }
        else if( cmd == "L"){   // After sending a race length, it will send this confirmation
            console() << "SerialReader :: Race Length " << args << endl;
        }
        else if( cmd == "M"){
            console() << "SerialReader :: Mock mode turned " << args << endl;
        }
        else if( cmd == "V"){   // version
            mFirmwareVersion = args;
        }
        
        else{
            console() << "SerialReader :: Unrecognized command :: '" << cmd << "'";
            if( args!=""){
                console() << " with arg :: '" << args << "'";
            }
            console() << endl;
        }
    }
    

}





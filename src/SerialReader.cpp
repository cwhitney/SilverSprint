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
//    BAUD_RATE(115200),
    BAUD_RATE(9600),
    bSerialConnected(false),
    mStringBuffer(""),
    mHardwareVersion(""),
    mProtoculVersion(""),
    mFirmwareVersion(""),
    bMockEnabled(false),
    mSerial(NULL) {
 
}

SerialReader::~SerialReader(){
    stopRace();
    
    if(mSerial != NULL ){
        try{
            delete mSerial;
        }catch(...){}
    }
}

void SerialReader::setup(){
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    
    int i = 0;
    const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		console() << "GoldsprintsFx :: [" << i++ << "] " << deviceIt->getName() << endl;
	}
    
    try {
//        Serial::Device dev = Serial::findDeviceByNameContains("tty.usbmodem");
        Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial");
		mSerial = new Serial( dev, BAUD_RATE );
        
        console() << "GoldsprintsFx :: OpenSprints hardware found successfully. :: " << dev.getName() << endl;
        bSerialConnected = true;
        
//        getHardwareVersion();
//        getProtoculVersion();
//        getFirmwareVersion();
	}
	catch( ... ) {
		console() << "GoldsprintsFx :: Couldn't connect to the OpenSprints hardware." << std::endl;
	}
}

void SerialReader::update() {
    if( bSerialConnected ){
        readSerial();
    }
}

void SerialReader::pingSensor() {
    mSerial->writeString("a:12345\n");
}

void SerialReader::startRace(){
//    getRaceLength();
    
    sendSerialMessage("g");
}

void SerialReader::stopRace(){
    sendSerialMessage("s");
}

void SerialReader::resetHardwareToDefault(){
//    sendSerialMessage("defaults\n");
}

void SerialReader::setRaceDuration(int numSeconds){
    sendSerialMessage("t:" + to_string(numSeconds) );
}

void SerialReader::getHardwareVersion(){
    sendSerialMessage("hw");
}

void SerialReader::getProtoculVersion(){
    sendSerialMessage("p");
}

void SerialReader::getFirmwareVersion(){
    sendSerialMessage("v");
}

void SerialReader::setRaceLengthTicks( int numTicks ){
//    console() << "Serial :: set num ticks " << numTicks << endl;
//    mSerial->writeString("l:" + to_string(numTicks) + "\n");
    sendSerialMessage( "l"+to_string(numTicks) );
}

void SerialReader::getRaceLength() {
//    mSerial->writeString("getlen\n");
}

void SerialReader::setMockMode( bool enabled ){
    bMockEnabled = enabled;
//    string mockStr = "m:" + (string)((bMockEnabled) ? "ON" : "OFF") + "\n";
//    mSerial->writeString(mockStr);
    sendSerialMessage("m");
}

void SerialReader::setCountdown( int numCountdownSeconds ){
    sendSerialMessage("c:" + to_string(numCountdownSeconds) );
}

void SerialReader::sendSerialMessage( std::string msg ){
    if( bSerialConnected ){
        mSerial->writeString(msg + "\n" );
    }else{
        console() << "SerialReader :: ERROR :: Hardware not connected" << endl;
    }
}

void SerialReader::readSerial(){
    uint bytesAvailable = mSerial->getNumBytesAvailable();
    uint charsAvailable = floor(bytesAvailable / sizeof(char));
    
    for(int i=0; i<charsAvailable; i++){
        unsigned char c = mSerial->readByte();
        mStringBuffer += c;
    }
    
    int index = mStringBuffer.find("\r\n");
    if(index != string::npos){
        std::string cmd = mStringBuffer.substr(0, index);
        mStringBuffer.replace(0, index+2, "");      // The newline is 2 bytes (\r\n) so add two to remove it
        
        parseCommand( cmd );
    }
}

bool SerialReader::isRaceFinished() {
    for( int i=0; i<mModel->playerData.size(); i++){
        if( !mModel->playerData[0]->isFinished() ){
            return false;
        }
    }
        
    return true;
}

void SerialReader::parseCommand(std::string command){
    if( command == "NACK" ){
        console() << "SerialReader :: Bad command" << endl;
    }else{
        std::vector<std::string> strs;
        boost::split(strs, command, boost::is_any_of(":"));
        
        std::string cmd = strs[0];
        std::string args = "";
        if( strs.size() > 1){
            args = strs[1];
//            boost::trim(args);
        }
        
        boost::to_upper(cmd);
        boost::to_upper(args);
        
        if( cmd == ""){
            return;
        }
        
        if( cmd == "F"){
            console() << "RACER FINISHED" << endl;
        }
        
        // ------------------------------------------------------------------------------
        // RACE FINISH
        if( cmd == "0F"){
            mModel->playerData[0]->setFinished( fromString<int>(args) );
            console() << "RACER 1 FINISHED " << args << endl;
            if( isRaceFinished() ){ stopRace(); }
        }
        else if( cmd == "1F"){
            mModel->playerData[1]->setFinished( fromString<int>(args) );
            console() << "RACER 2 FINISHED " << args << endl;
            if( isRaceFinished() ){ stopRace(); }
        }
        else if( cmd == "2F"){
            mModel->playerData[2]->setFinished( fromString<int>(args) );
            console() << "RACER 3 FINISHED " << args << endl;
            if( isRaceFinished() ){ stopRace(); }
        }
        else if( cmd == "3F"){
            mModel->playerData[3]->setFinished( fromString<int>(args) );
            console() << "RACER 4 FINISHED " << args << endl;
            if( isRaceFinished() ){ stopRace(); }
        }
        // ------------------------------------------------------------------------------
        // RACE PROGRESS
        else if( cmd == "0"){
            mModel->playerData[0]->curRaceTicks = fromString<int>(args);
        }
        else if( cmd == "1"){
            mModel->playerData[1]->curRaceTicks = fromString<int>(args);
        }
        else if( cmd == "2"){
            mModel->playerData[2]->curRaceTicks = fromString<int>(args);
        }
        else if( cmd == "3"){
            mModel->playerData[3]->curRaceTicks = fromString<int>(args);
        }
        else if( cmd == "T"){
            mModel->elapsedRaceTimeMillis = fromString<int>(args);
        }
        
        // ------------------------------------------------------------------------------
        // SETTINGS
        else if( cmd == "A"){
            console() << "SerialReader :: Got ping :: " << args << endl;
        }
        else if( cmd == "C"){
            console() << "SerialReader :: Countdown changed to " << args << " seconds" << endl;
        }
        else if( cmd == "CD"){
            console() << "SerialReader :: Countdown :: " << args << endl;
        }
        else if( cmd == "DEFAULTS"){
            console() << "SerialReader :: Hardware reset to defaults" << endl;
        }
        else if( cmd == "F"){
            console() << "SerialReader :: False start. Racer: " << args << endl;    // 0 based
        }
        else if( cmd == "FS"){
            console() << "SerialReader :: False start. Racer: " << args << endl;    // 0 based
        }
        else if( cmd == "G"){
            console() << "SerialReader :: RACE STARTING" << endl;
        }
        else if( cmd == "HW"){
            mHardwareVersion = args;
        }
        else if( cmd == "L"){
//            mModel->setRaceLengthMeters( fromString<int>(args) );
            console() << "SerialReader :: Race Length " << args << endl;
        }
        else if( cmd == "M"){
            console() << "SerialReader :: Mock mode turned " << args << endl;
        }
        else if( cmd == "P"){
            mProtoculVersion = args;
        }
        else if( cmd == "S"){
            console() << "SerialReader :: Countdown stopped" << endl;
        }
        else if( cmd == "V"){
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





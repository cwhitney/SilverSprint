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
    mStringBuffer(""),
    mHardwareVersion(""),
    mProtoculVersion(""),
    mFirmwareVersion("") {
    
        
}

void SerialReader::setup(){
    mStateManager = StateManager::getInstance();
    
    int i = 0;
    const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		console() << "GoldsprintsFx :: [" << i++ << "] " << deviceIt->getName() << endl;
	}
    
    try {
//        Serial::Device dev = Serial::findDeviceByNameContains("tty.usbmodem");
        Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial");
		mSerial = Serial( dev, BAUD_RATE );
        
        console() << "GoldsprintsFx :: OpenSprints hardware found successfully. :: " << dev.getName() << endl;
        bSerialConnected = true;
        mSerial.flush();
        
        getHardwareVersion();
        getProtoculVersion();
        getFirmwareVersion();
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
    mSerial.writeString("!a:12345\n");
}

void SerialReader::startRace(){
    mSerial.writeString("!g\n");
}

void SerialReader::stopRace(){
    mSerial.writeString("!s\n");
}

void SerialReader::resetHardwareToDefault(){
    mSerial.writeString("!defaults\n");
}

void SerialReader::setRaceDuration(int numSeconds){
    mSerial.writeString("!t:" + to_string(numSeconds) + "\n");
}

void SerialReader::getHardwareVersion(){
    mSerial.writeString("!hw\n");
}

void SerialReader::getProtoculVersion(){
    mSerial.writeString("!p\n");
}

void SerialReader::getFirmwareVersion(){
    mSerial.writeString("!v\n");
}

void SerialReader::setMockMode(){
    mSerial.writeString("!m\n");
}

void SerialReader::setCountdown( int numCountdownSeconds ){
    console() << "Setting countdown to " << numCountdownSeconds << endl;
    mSerial.writeString("!c:" + to_string(numCountdownSeconds) + "\n");
}

void SerialReader::readSerial(){
    uint bytesAvailable = mSerial.getNumBytesAvailable();
    uint charsAvailable = floor(bytesAvailable / sizeof(char));
    
    for(int i=0; i<charsAvailable; i++){
        unsigned char c = mSerial.readByte();
        mStringBuffer += c;
    }
    
    int index = mStringBuffer.find("\r\n");
    if(index != string::npos){
        std::string cmd = mStringBuffer.substr(0, index);
        mStringBuffer.replace(0, index+2, "");      // The newline is 2 bytes (\r\n) so add two to remove it
        
        console() << "cmd :: " << cmd << endl;
        
        parseCommand( cmd );
    }
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
        }
        
        // ------------------------------------------------------------------------------
        
        if( cmd == "0F"){
            console() << "SerialReader :: Racer 1 Finished :: " << args << endl;
        }
        else if( cmd == "1F"){
            console() << "SerialReader :: Racer 2 Finished :: " << args << endl;
        }
        else if( cmd == "2F"){
            console() << "SerialReader :: Racer 3 Finished :: " << args << endl;
        }
        else if( cmd == "3F"){
            console() << "SerialReader :: Racer 4 Finished :: " << args << endl;
        }
        
        // ------------------------------------------------------------------------------
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
        else if( cmd == "HW"){
            mHardwareVersion = args;
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





//
//  SerialReader.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "data/SerialReader.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;
using namespace Cinder::Serial;

SerialReader::SerialReader() :
    BAUD_RATE(115200),
    mStringBuffer(""),
    mHardwareVersion(""),
    mProtoculVersion(""),
    mFirmwareVersion(""),
    mSerial(NULL),
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
		mSerial->flush();
        onConnect();
        timeline().add( [&](){ getVersion(); }, timeline().getCurrentTime() + 2.0 );
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
			auto ports = SerialPort::getPorts(true);
            /*
			for (auto port : ports) {
				console() << "SERIAL DEVICE" << endl;
				console() << "\tNAME: " << port->getName() << endl;
				console() << "\tDESCRIPTION: " << port->getDescription() << endl;
				console() << "\tHARDWARE IDENTIFIER: " << port->getHardwareIdentifier() << endl;
			}
             */
			try {
				if (!ports.empty()) {
					SerialPortRef port = SerialPort::findPortByDescriptionMatching(std::regex("Arduino.*"), true);
					if (!port) {
						port = ports.back();
					}

					mSerial = SerialDevice::create(port, BAUD_RATE);
					mSerial->flush();
					bSerialConnected = true;
					
					for (int i = 0; i < mSendBuffer.getSize(); i++) {        // clear send buffer
						std::string tmp;
						mSendBuffer.popBack(&tmp);
					}
					stopRace();

					for (int i = 0; i < mReceiveBuffer.getSize(); i++) {    // clear receive buffer
						std::vector<std::string> tmp;
						mReceiveBuffer.popBack(&tmp);
					}
					CI_LOG_I("OpenSprints hardware found successfully. :: ") << mSerial->getPortName();
				}
			}catch (serial::IOException& e) {
				if (mSerial && mSerial->isOpen()) {
					mSerial->close();
				}
                bSerialConnected = false;
            }
        }else{
            // make sure we're still connected
			auto pOpen = SerialPort::findPortByDescriptionMatching(std::regex("Arduino.*"), true);

            if(pOpen == nullptr){
				if (mSerial && mSerial->isOpen()) {
					mSerial->close();
				}
                bSerialConnected = false;
            }else{ 
				// send to arduino
                for(int i=0; i<mSendBuffer.getSize(); i++){
                    std::string msgToSend;
                    mSendBuffer.popBack( &msgToSend );

					CI_LOG_I("Sending :: ") << msgToSend;

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
    try {
		size_t bytesAvailable = mSerial->getNumBytesAvailable();
		size_t charsAvailable = floor(bytesAvailable / sizeof(char));

        for(int i=0; i<charsAvailable; i++){
			unsigned char c;	// uint8_t
			mSerial->readBytes(&c, 1);
            mStringBuffer += c;
        }
    }catch(serial::IOException& e){
        bSerialConnected = false;
		if (mSerial && mSerial->isOpen()) {
			mSerial->close();
			//mSerial = nullptr;
		}
    }
    
    auto index = mStringBuffer.find("\r\n");
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
    auto numCmds = mReceiveBuffer.getSize();
    
    for( int i=0; i<numCmds; i++) {
        std::vector<std::string> cmdArgs;
        mReceiveBuffer.popBack( &cmdArgs );
        
        std::string cmd = cmdArgs[0];
        std::string args = (cmdArgs.size() > 1) ? cmdArgs[1] : "";
        
        if(cmd == ""){
            return;
        }
        
        // ------------------------------------------------------------------------------
        // RACE FINISH (ars are the time the race finished in millis)
        if(cmd == "0F"){
            CI_LOG_I("RACER 1 FINISHED " + args);
            mStateManager->signalRacerFinish.emit(0, fromString<int>(args), mModel->playerData[0]->getCurrentRaceTicks());
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        else if( cmd == "1F"){
            CI_LOG_I("RACER 2 FINISHED " +args);
            mStateManager->signalRacerFinish.emit(1, fromString<int>(args), mModel->playerData[1]->getCurrentRaceTicks());
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        else if( cmd == "2F"){
            CI_LOG_I("RACER 3 FINISHED " + args);
            mStateManager->signalRacerFinish.emit(2, fromString<int>(args), mModel->playerData[2]->getCurrentRaceTicks());
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        else if( cmd == "3F"){
            CI_LOG_I("RACER 4 FINISHED " + args);
            mStateManager->signalRacerFinish.emit(3, fromString<int>(args), mModel->playerData[3]->getCurrentRaceTicks());
            if( isRaceFinished() ){ mStateManager->signalOnRaceFinished.emit(); }
        }
        
        // ------------------------------------------------------------------------------
        // RACE PROGRESS (args are race ticks, then race time millis)
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
            CI_LOG_I("SerialReader :: Countdown :: " + args);
            
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
            CI_LOG_I("SerialReader :: False start. Racer: " + args);    // 0 based
//            mStateManager->changeRaceState( RACE_STATE::RACE_FALSE_START );
//            mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED );
        }
        else if( cmd == "L"){   // After sending a race length, it will send this confirmation
            CI_LOG_I("SerialReader :: Race Length " + args + ".");
        }
        else if( cmd == "M"){   // Mock mode confirmation
            CI_LOG_I("SerialReader :: Mock mode turned " + args);
        }
        else if(cmd == "V"){   // version
            mFirmwareVersion = args;
            mModel->mFirmwareVersion = args;
        }
        
        else{
            CI_LOG_I("SerialReader :: Unrecognized command :: ");
			try {
				CI_LOG_I(cmd);
			}catch (...) {}

            if(args != ""){
				try {
					CI_LOG_I(" with arg :: '" + args + "'");
				}catch (...) {}
            }
        }
    }
}

void SerialReader::onConnect()
{
    CI_LOG_I("OpenSprints hardware connected.");
    mModel->bHardwareConnected = bSerialConnected;
    mModel->mFirmwareVersion = "Unknown";
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
    sendSerialMessage("t" + to_string(numSeconds) );
}

void SerialReader::setRaceLengthTicks( int numTicks ){
    sendSerialMessage( "l"+to_string(numTicks) );
}

void SerialReader::setMockMode( bool enabled ){
    bMockEnabled = enabled;
    sendSerialMessage("m");
}

void SerialReader::getVersion(){
    sendSerialMessage("v");
}

void SerialReader::setRaceType( gfx::RACE_TYPE raceType)
{
    if(raceType == RACE_TYPE_DISTANCE){
        sendSerialMessage("d");
    }else{
        sendSerialMessage("x");
    }
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




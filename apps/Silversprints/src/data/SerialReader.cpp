//
//  SerialReader.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "data/SerialReader.h"
#include "data/StateManager.h"
#include "cinder/Timeline.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;
using namespace Cinder::Serial;

SerialReader::SerialReader() :
	BAUD_RATE(115200),
	mReceiveBuffer(100),
	mSendBuffer(100)
{ }

SerialReader::~SerialReader()
{
	ThreadSetup threadSetup;
	std::scoped_lock<std::mutex> guard(mSerialMutex);
	
    bThreadShouldQuit = true;
	
	if (mSerialThreadPtr != nullptr && mSerialThreadPtr->joinable()) {
		mSerialThreadPtr->join();
		mSerialThreadPtr = nullptr;
	}
}

void SerialReader::setup()
{
	mSerialThreadPtr = std::make_unique<std::thread>( &SerialReader::updateSerialThread, this );
}

void SerialReader::update()
{
    // if we connected since the last update, notify
    if( bSerialConnected && !bLastConnection ){
        onConnect();

    	// Wait 2 seconds, and then get the version
        timeline().add( [&](){ getVersion(); }, timeline().getCurrentTime() + 2.0f );
    }else if( !bSerialConnected && bLastConnection ){
        onDisconnect();
    }
    bLastConnection = bSerialConnected;
    
    // Parse any data that's been read in on the other thread
    parseFromBuffer();
}

void SerialReader::selectSerialDevice(const std::string &portName)
{
    std::lock_guard<std::mutex> guard(mSerialMutex);
	
    CI_LOG_I("Select serial device :: " << portName);
    mConnectedPortName = portName;
    
    if (mSerial && mSerial->isOpen()) {
        if(mSerial->getPortName() != portName){
            mSerial->close();
            bSerialConnected = false;
        }
    }
}

void SerialReader::updateSerialThread()
{
    ThreadSetup threadSetup;
    
    while( !bThreadShouldQuit ){
    	std::scoped_lock<std::mutex> guard(mSerialMutex);
        
        if(!bSerialConnected){ // We're diconnected, try to connect
			if( reconnectSerialDevice() ){
				mSendBuffer.clear();
				
				stopRace();
				getVersion();

				// is there a reason this is cleared after?
				mReceiveBuffer.clear();
				
				bForceSerialDescUpdate = true;
				CI_LOG_I("OpenSprints hardware found successfully. :: ") << mSerial->getPortName();
				Model::instance().setSerialPortName( mSerial->getPortName() );
			}else {
		         // If we couldn't connect, sleep for 500ms before retrying
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }else {
            if( getElapsedSeconds() - mLastKeepAlive > 1.0){
                // sendKeepAlive();
            	// updatePortDescriptions();
            	mLastKeepAlive = getElapsedSeconds();
            }
           
			if (mSerial->isOpen()) {
				sendBufferToDevice();
				readBufferFromDevice();
			}
			else {
				CI_LOG_W("Serial is not open");
			}
        }
    }
}

bool SerialReader::reconnectSerialDevice()
{
	updatePortDescriptions();
	auto ports = SerialPort::getPorts(true);
	if (ports.empty()) {
		CI_LOG_I("No serial ports found");
		return false;
	}
	printDevices(ports);

	// This will be default try to find an Arduino, or another device if you've selected one in the dropdown
	auto port = SerialPort::findPortByNameMatching(std::regex(mConnectedPortName), true);

	// If we didn't strictly find any arduinos, just display the last detected port
	if(port == nullptr){
		port = ports.back();
	}
	
	// bForceSerialDescUpdate = true;

	// if (mSerial && mSerial->isOpen()) {
	// 	mSerial->close();
	// 	mSerial = nullptr;
	// 	// return false;
	// }

	try	{
		mSerial = SerialDevice::create(port, BAUD_RATE);
		bSerialConnected = true;
	}catch (Exception &e)	{
		bSerialConnected = false;
		CI_LOG_EXCEPTION("Exc: Error opening serial port", e)
		return false;
	}catch (serial::IOException &e)	{
		bSerialConnected = false;
		CI_LOG_EXCEPTION("IOExc: Error opening serial port", e)
		return false;
	}

	return true;
}

void SerialReader::printDevices( const vector<SerialPortRef> &ports)
{
	for (auto port : ports) {
		CI_LOG_I("SERIAL DEVICE");
		CI_LOG_I("\tNAME: ")<< port->getName() << "\n";
		CI_LOG_I("\tDESCRIPTION: ")<< port->getDescription() << "\n";
		CI_LOG_I("\tHARDWARE IDENTIFIER: ")<< port->getHardwareIdentifier() << "\n";
	}
}


void SerialReader::sendBufferToDevice()
{
	// Send queued commands to arduino
	if (mSendBuffer.getSize() > 0) {
		CI_LOG_I("Sending buffer over to arduino: ") << mSendBuffer.getSize() << " messages";
	}

	while (mSendBuffer.getSize() > 0) {
		std::string msgToSend;
		mSendBuffer.popBack(&msgToSend);
		if (!msgToSend.empty()){
			CI_LOG_I("Sending :: ") << msgToSend << " :: " << mSerial->isOpen();
			mSerial->writeString(msgToSend);
		}
	}

	/*for (size_t i = 0; i < mSendBuffer.getSize(); i++) {
		std::string msgToSend;
		mSendBuffer.popBack(&msgToSend);
		if (!msgToSend.empty()){
			CI_LOG_I("Sending :: ") << msgToSend << " :: " << mSerial->isOpen();
			mSerial->writeString(msgToSend);
		}
	}*/
}

void SerialReader::sendKeepAlive()
{
    // TODO: Do we need a heartbeat in the event the connection goes idle?
}

void SerialReader::updatePortDescriptions()
{
    // update the model with what's connected
    auto ports = SerialPort::getPorts(true);
	bForceSerialDescUpdate = true; // force for now
	
    if(bForceSerialDescUpdate || ports.size() != Model::instance().mSerialDeviceList.size()){
        bForceSerialDescUpdate = false;
        std::vector<Model::SerialDeviceDesc> portList;
        for (auto port : ports) {
            Model::SerialDeviceDesc sdd;
            sdd.portName = port->getName();
            sdd.portDescription = port->getDescription();
            portList.push_back(sdd);
        }

    	if (portList.empty()) {
    		Model::SerialDeviceDesc sdd;
    		sdd.portName = "No hardware found";
    		sdd.portDescription = "";
    		portList.push_back(sdd);
    	}
        
        Model::instance().mSerialDeviceList = portList;
        StateManager::instance().signalSerialDevicesUpdated.emit();
    }
}

void SerialReader::readBufferFromDevice()
{
    try {
        size_t bytesAvailable = mSerial->getNumBytesAvailable();
        size_t charsAvailable = floor(bytesAvailable / sizeof(char));
        
        for(size_t i=0; i<charsAvailable; i++){
            unsigned char c;    // uint8_t
            mSerial->readBytes(&c, 1);
            mStringBuffer += c;
        }
    }catch(serial::IOException& e){
    	CI_LOG_D("We've Disconnected! " << e.what());
        bSerialConnected = false;
        if (mSerial && mSerial->isOpen()) {
            try{
                mSerial->close();
            }catch(serial::IOException &exc){
                CI_LOG_EXCEPTION("Error closing serial port", exc);
            }
            Model::instance().mSerialConnectionState = Model::SerialConnectionState::DISCONNECTED;
            //mSerial = nullptr;
        }
    }
    
    auto index = mStringBuffer.find("\r\n");
    if(index != string::npos){
        std::string cmd = mStringBuffer.substr(0, index);
        mStringBuffer.replace(0, index+2, "");      // The newline is 2 chars (\r\n) so add two to remove it
        
        parseCommandToBuffer( cmd );
    }
}

void SerialReader::parseCommandToBuffer( std::string command )
{
    std::vector<std::string> strs = ci::split( command, ':');    
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
    
	for (int i = 0; i < numCmds; i++) {
		std::vector<std::string> cmdArgs;
		mReceiveBuffer.popBack(&cmdArgs);

		std::string cmd = cmdArgs[0];
		std::string args = (cmdArgs.size() > 1) ? cmdArgs[1] : "";

		if (cmd.empty()) {
			return;
		}
		// ------------------------------------------------------------------------------
		// KIOSK MODE
		if (cmd == "G") {
			if (StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_STOPPED) {
				StateManager::instance().changeRaceState(RACE_STATE::RACE_STARTING);
			}
		}
		else if (cmd == "S") {
			StateManager::instance().changeRaceState(RACE_STATE::RACE_STOPPED);
		}

		// ------------------------------------------------------------------------------
		// RACE FINISH (ars are the time the race finished in millis)
		else if (cmd == "0F") {
			CI_LOG_I("RACER 1 FINISHED " + args);
			StateManager::instance().signalRacerFinish.emit(0, fromString<int>(args), Model::instance().playerData[0]->getCurrentRaceTicks());
			if (isRaceFinished()) { StateManager::instance().signalOnRaceFinished.emit(); }
		}
		else if (cmd == "1F") {
			CI_LOG_I("RACER 2 FINISHED " + args);
			StateManager::instance().signalRacerFinish.emit(1, fromString<int>(args), Model::instance().playerData[1]->getCurrentRaceTicks());
			if (isRaceFinished()) { StateManager::instance().signalOnRaceFinished.emit(); }
		}
		else if (cmd == "2F") {
			CI_LOG_I("RACER 3 FINISHED " + args);
			StateManager::instance().signalRacerFinish.emit(2, fromString<int>(args), Model::instance().playerData[2]->getCurrentRaceTicks());
			if (isRaceFinished()) { StateManager::instance().signalOnRaceFinished.emit(); }
		}
		else if (cmd == "3F") {
			CI_LOG_I("RACER 4 FINISHED " + args);
			StateManager::instance().signalRacerFinish.emit(3, fromString<int>(args), Model::instance().playerData[3]->getCurrentRaceTicks());
			if (isRaceFinished()) { StateManager::instance().signalOnRaceFinished.emit(); }
		}

		// ------------------------------------------------------------------------------
		// RACE PROGRESS (rdata should contain 4 comma separated values, one for each racer, followed by the raceTimeMillis)
		else if (cmd == "R") {
			std::vector<std::string> rdata = ci::split(args, ',');
			if (rdata.empty()) {
				CI_LOG_W("Empty race data received");
				continue;
			}
			
			int raceMillis = fromString<int>(rdata.back());

			// There is always guaranteed to be data for 4 racers, even when the race is set to fewer
			for (size_t k = 0; k < 4; k++) {
				Model::instance().playerData[k]->updateRaceTicks(fromString<int>(rdata[k]), raceMillis);
			}

			Model::instance().elapsedRaceTimeMillis = raceMillis;
			Model::instance().startTimeMillis = ci::app::getElapsedSeconds() * 1000.0 - Model::instance().elapsedRaceTimeMillis;
		}

		// ------------------------------------------------------------------------------
		// SETTINGS
		else if (cmd == "CD") {
			CI_LOG_I("SerialReader :: Countdown :: " + args);

			if (args == "3") {
				StateManager::instance().changeRaceState(RACE_STATE::RACE_COUNTDOWN_3);
			}
			else if (args == "2") {
				StateManager::instance().changeRaceState(RACE_STATE::RACE_COUNTDOWN_2);
			}
			else if (args == "1") {
				StateManager::instance().changeRaceState(RACE_STATE::RACE_COUNTDOWN_1);
			}
			else if (args == "0") {
				Model::instance().startTimeMillis = ci::app::getElapsedSeconds() * 1000.0;

				StateManager::instance().changeRaceState(RACE_STATE::RACE_COUNTDOWN_GO);
				StateManager::instance().changeRaceState(RACE_STATE::RACE_RUNNING);
			}
		}
		else if (cmd == "FS") {
			CI_LOG_I("SerialReader :: False start. Racer: " + args);    // 0 based
			//            StateManager::instance().changeRaceState( RACE_STATE::RACE_FALSE_START );
			//            StateManager::instance().changeRaceState( RACE_STATE::RACE_STOPPED );
		}
		else if (cmd == "L") {   // After sending a race length, it will send this confirmation
			CI_LOG_I("SerialReader :: Race Length " + args + ".");
		}
		else if (cmd == "M") {   // Mock mode confirmation
			CI_LOG_I("SerialReader :: Mock mode turned " + args);
		}
		else if (cmd == "V") {   // version
			mFirmwareVersion = args;
			Model::instance().mFirmwareVersion = args;
			Model::instance().mSerialConnectionState = Model::SerialConnectionState::CONNECTED_SILVERSPRINTS;
		}

		else {
			CI_LOG_I("SerialReader :: Unrecognized command :: ");
		}
    }
}

void SerialReader::onConnect()
{
    CI_LOG_I("OpenSprints hardware connected.");
    Model::instance().setHardwareConnected(bSerialConnected);
    Model::instance().mFirmwareVersion = "Unknown";
    
    Model::instance().mSerialConnectionState = Model::SerialConnectionState::CONNECTED_UNKNOWN;
}

void SerialReader::onDisconnect()
{
    CI_LOG_I("OpenSprints hardware disconnected.");
    Model::instance().setHardwareConnected(bSerialConnected);
    
    Model::instance().mSerialConnectionState = Model::SerialConnectionState::DISCONNECTED;
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

void SerialReader::setRaceType( Model::RACE_TYPE raceType)
{
    if(raceType == Model::RACE_TYPE_DISTANCE){
        sendSerialMessage("d");
    }else{
        sendSerialMessage("x");
    }
}

void SerialReader::sendSerialMessage( std::string msg )
{
	CI_LOG_I( "Sending serial message :: ") << msg << "\n";
	
    std::string toSend = msg + '\n';
    mSendBuffer.pushFront( toSend );
}

bool SerialReader::isRaceFinished()
{
    for( int i=0; i<Model::instance().getNumRacers(); i++){
        if( !Model::instance().playerData[i]->isFinished() ){
            return false;
        }
    }
    
    return true;
}



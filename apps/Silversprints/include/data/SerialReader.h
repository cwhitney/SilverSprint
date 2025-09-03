//
//  SerialReader.h
//  SilverSprints
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "cinder/Log.h"
#include "cinder/ConcurrentCircularBuffer.h"
#include "data/Model.h"
#include "Cinder-Serial.h"

namespace gfx
{    
	using SerialReaderRef = std::shared_ptr<class SerialReader>;
	class SerialReader
	{
	  public:
		SerialReader();
		~SerialReader();
		void setup();
		void update();
		
		void startRace();
		void stopRace();
		
		void setRaceDuration( int numSeconds );
		void setRaceType(Model::RACE_TYPE raceType);
		void setRaceLengthTicks(int numTicks);
		void setMockMode(bool enabled=true);
		void getVersion();
		
		void selectSerialDevice(const std::string &portName);
		
	  private:
		void updateSerialThread();
		bool reconnectSerialDevice();
		
		void sendBufferToDevice();
		void readBufferFromDevice();

		void sendKeepAlive();
		void updatePortDescriptions();
		bool deviceListsAreEqual(const std::vector<Model::SerialDeviceDesc>& a, const std::vector<Model::SerialDeviceDesc>& b);
		
		void onConnect();
		void onDisconnect();
		
		void parseCommandToBuffer(std::string command);
		void parseFromBuffer();

		void printDevices(const std::vector<Cinder::Serial::SerialPortRef> &ports);
		
		bool isRaceFinished();
		
		void sendSerialMessage( std::string msg );
		
		int BAUD_RATE = 115200;
		
		Cinder::Serial::SerialDeviceRef mSerial = nullptr;
		bool bForceSerialDescUpdate = false;
		
		bool bSerialConnected = false, bLastConnection = false;
		bool bThreadShouldQuit = false;
		bool bMockEnabled = false;
		
		std::string mStringBuffer;
		std::string mFirmwareVersion;
		std::string mConnectedPortName = "Arduino.*";
		double mLastKeepAlive = 0.0;
		
		// threading
		std::unique_ptr<std::thread> mSerialThreadPtr;
		std::mutex mSerialMutex;
		ci::ConcurrentCircularBuffer<std::vector<std::string>> mReceiveBuffer;
		ci::ConcurrentCircularBuffer<std::string> mSendBuffer;
	};
}

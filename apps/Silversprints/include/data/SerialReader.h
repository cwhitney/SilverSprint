//
//  SerialReader.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/App.h"
//#include "cinder/Serial.h"
#include "cinder/Utilities.h"
#include "cinder/Log.h"
#include "cinder/Timeline.h"
#include "cinder/ConcurrentCircularBuffer.h"
#include <boost/algorithm/string.hpp>

#include "data/StateManager.h"
#include "data/GFXGlobal.h"
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
        
        void setRaceDuration( int );
        void setRaceType( gfx::RACE_TYPE raceType);
        void setRaceLengthTicks( int numTicks );
        void setMockMode( bool enabled=true );
        void getVersion();
        
      private:
        void updateSerialThread();
        
        void onConnect();
        void onDisconnect();
        
        void parseCommandToBuffer( std::string command );
        void parseFromBuffer();
        
        bool                isRaceFinished();
        
        StateManager        *mStateManager;
        
        void                readSerial();
        void                sendSerialMessage( std::string msg );
        
        int                 BAUD_RATE = 115200;
        //ci::SerialRef       mSerial;
		Cinder::Serial::SerialDeviceRef		mSerial;
        bool                bSerialConnected = false, bLastConnection = false;
        bool                bThreadShouldQuit = false;
        bool                bMockEnabled = false;
        
        std::string         mStringBuffer;
        
        std::string         mFirmwareVersion;
        std::string         mProtoculVersion;
        std::string         mHardwareVersion;   // currently unimplemented in arduino software
        
        // threading
        std::shared_ptr<std::thread>    mSerialThread;
        std::mutex                      mSerialMutex;
        ci::ConcurrentCircularBuffer<std::vector<std::string>>	mReceiveBuffer;
        ci::ConcurrentCircularBuffer<std::string>               mSendBuffer;
    };
}

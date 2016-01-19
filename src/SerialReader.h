//
//  SerialReader.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Serial.h"
#include "cinder/Utilities.h"
#include "cinder/Log.h"
#include "cinder/ConcurrentCircularBuffer.h"
#include <boost/algorithm/string.hpp>

#include "StateManager.h"
#include "Model.h"

namespace gfx {
    
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
        void setRaceLengthTicks( int numTicks );
        void setMockMode( bool enabled=true );
        
      private:
        void updateSerialThread();
        
        void onConnect();
        void onDisconnect();
        
        void parseCommandToBuffer( std::string command );
        void parseFromBuffer();
        
        bool                isRaceFinished();
        
        StateManager        *mStateManager;
        Model               *mModel;
        
        void                readSerial();
        void                sendSerialMessage( std::string msg );
        
        int                 BAUD_RATE;
        ci::SerialRef       mSerial;
        bool                bSerialConnected, bLastConnection;
        bool                bThreadShouldQuit;
        bool                bMockEnabled;
        
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
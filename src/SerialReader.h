//
//  SerialReader.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Serial.h"
#include <boost/algorithm/string.hpp>

#include "StateManager.h"

namespace gfx {
    class SerialReader {
      
      public:
        SerialReader();
        void setup();
        void update();
        
        void setCountdown( int numCountdownSeconds );
        void setRaceDuration( int );
        void startRace();
        void stopRace();
        void resetHardwareToDefault();
        void getHardwareVersion();
        void getProtoculVersion();
        void getFirmwareVersion();
        void setMockMode();
        void pingSensor();
        
      private:
        StateManager        *mStateManager;
        
        void                readSerial();
        void                parseCommand( std::string command );
        
        int                 BAUD_RATE;
        ci::Serial          mSerial;
        bool                bSerialConnected;
        
        std::string         mStringBuffer;
        
        std::string         mFirmwareVersion;
        std::string         mProtoculVersion;
        std::string         mHardwareVersion;   // currently unimplemented in arduino software
    };
}
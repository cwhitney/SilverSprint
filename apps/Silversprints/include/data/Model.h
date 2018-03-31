//
//  Model.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Font.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include "StateManager.h"
#include "PlayerData.h"

namespace gfx {
    
    class Model {
        friend class SerialReader;
        
    public:
        
        static Model &instance(){
            static Model sInstance;
            return sInstance;
        }
        
        enum RACE_TYPE {
            RACE_TYPE_TIME = 0,
            RACE_TYPE_DISTANCE
        };
        const RACE_TYPE& getCurrentRaceType(){ return mCurrentRaceType; }
        void setCurrentRaceType(const RACE_TYPE &type){ mCurrentRaceType = type; }
        
        void resetPlayers();
        
        void setRaceLengthMeters( const float &meters );
        const float& getRaceLengthMeters(){ return RaceSettings.raceLengthMeters; }
        const int& getRaceLengthTicks(){ return RaceSettings.totalRaceTicks; }
        
        void setNumRacers( const int &num ){ RaceSettings.numRacers = num; }
        const int& getNumRacers(){ return RaceSettings.numRacers; }
        
        void setRollerDiameterMm( const float &mm );
        const float& getRollerDiameterMm(){ return RaceSettings.mRollerDiameterMm; }
        
        void setRaceTimeSeconds(const double &seconds){ RaceSettings.mRaceLengthMillis = seconds * 1000.0; };
        const double getRaceTimeSeconds(){ return RaceSettings.mRaceLengthMillis / 1000.0; };
        
        void setHardwareConnected(const bool &connected){ RaceSettings.bHardwareConnected = connected; }
        const bool& isHardwareConnected(){ return RaceSettings.bHardwareConnected; }
        PlayerData* getPlayerData(const int &num){ return playerData[num]; };
        
        const double& getRaceLengthMillis(){ return RaceSettings.mRaceLengthMillis;}
        
        void setUseKph(bool useKph){ RaceSettings.bUseKph = useKph;}
        bool getUsesKph(){ return RaceSettings.bUseKph; }
        
        void setRaceLogging(const bool &bShouldLog){
            RaceSettings.bLogRacesToFile = bShouldLog;
        }
        const bool getRaceLogging(){ return RaceSettings.bLogRacesToFile; }
        
        //! Set the currently selected serial ports name. Returns true if different from the current value. Returns false if it's the same.
        bool setSerialPortName(const std::string &portName){
            ci::app::console() << "Model serial port :: " << portName << std::endl;
            if(mSelectedPortName != portName){
                mSelectedPortName = portName;
                return true;
//                StateManager::instance().signalSerialDeviceSelected.emit(mSelectedPortName);
            }
            return false;
        }
        const std::string& getPortName(){ return mSelectedPortName; }
        
        enum SerialConnectionState {
            DISCONNECTED,
            CONNECTED_UNKNOWN,
            CONNECTED_SILVERSPRINTS
        };
        
        SerialConnectionState           mSerialConnectionState = SerialConnectionState::DISCONNECTED;
        
        std::vector<gfx::PlayerData*>   playerData;
        ci::Color                       playerColors[4];
        
        int                             startTimeMillis;
        int                             elapsedRaceTimeMillis;
        
        struct SerialDeviceDesc {
            std::string portName = "Unknown";
            std::string portDescription = "Unknown";
        };
        std::vector<SerialDeviceDesc>   mSerialDeviceList;
        std::string                     mFirmwareVersion = "Unknown";
        
        // FONTS
        ci::Font uiFont;
        ci::gl::TextureFontRef      texFont, winnerTexFont, winnerUiFont;
        ci::gl::TextureFontRef      countdownFont;
        
        ci::vec2   localToGlobal( ci::vec2 pos );
        void setScale( const ci::Rectf &scaledRect );
        
        //! How often the mph is updated for racers
        const float speedUpdateInterval = 0.5f;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        Model();
        ~Model();
        Model(Model const&){};
        void    setRaceLength(int ticks);
        
        RACE_TYPE mCurrentRaceType = RACE_TYPE::RACE_TYPE_TIME;
        
        struct RaceSettings {
            int numRacers = 2;
            // For distance based
            
            int     totalRaceTicks = 0;
            float   raceLengthMeters = 100;
            
            // For time based
            double  mRaceLengthMillis = 60000;
            
            float   mRollerDiameterMm = 114.3f;     // This is 4.5 inches in mm
            bool    bHardwareConnected = false;
            bool    bUseKph = true;
            bool    bLogRacesToFile = false;
        } RaceSettings;
        
        ci::vec2 mScreenScale, mScreenOffset;
        std::string     mSelectedPortName = "";
    };
}

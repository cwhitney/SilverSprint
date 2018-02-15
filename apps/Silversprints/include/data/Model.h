//
//  Model.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

#include "PlayerData.h"

namespace gfx {

    class Model {
        friend class SerialReader;
        
      public:

        static Model &instance(){
            static Model sInstance;
            return sInstance;
        }
        
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

//        bool isHardwareConnected(){ return RaceSettings.bHardwareConnected; }

        std::vector<gfx::PlayerData*>   playerData;
        ci::Color                       playerColors[4];
        
        int                             startTimeMillis;
        int                             elapsedRaceTimeMillis;
        std::string                     mFirmwareVersion = "Unknown";
        
      private:
        // PRIVATE CONSTRUCTOR + COPY
        Model();
        ~Model();
        Model(Model const&){};
        
        static Model    *mInstance;
        
        void    setRaceLength(int ticks);
        
        struct RaceSettings {
            int numRacers = 2;
            // For distance based
            
            int     totalRaceTicks = 0;
            float   raceLengthMeters = 100;
            
            // For time based
            double    mRaceLengthMillis = 60000;
            
            float   mRollerDiameterMm = 114.3f;     // This is 4.5 inches in mm
            bool    bHardwareConnected = false;
            bool    bUseKph = true;
        } RaceSettings;
        
       
        
        
    };
}

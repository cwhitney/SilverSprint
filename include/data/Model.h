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
        static Model* getInstance();
        void resetPlayers();
        
        void setRaceLengthMeters( const float &meters );
        const int& getRaceLengthTicks(){ return totalRaceTicks; }
        
        void setNumRacers( const int &num ){ mNumRacers = num; }
        const int& getNumRacers(){ return mNumRacers; }
        
        void setRollerDiameterMm( const float &mm );
        const float& getRollerDiameterMm(){ return mRollerDiameterMm; }
        
        const bool& isHardwareConnected(){ return bHardwareConnected; }
        PlayerData* getPlayerData(int num){ return playerData[num]; };
        
        void setUseKph(bool useKph){ bUseKph = useKph;}
        bool getUsesKph(){ return bUseKph; }

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
        
        int     mNumRacers;
        void    setRaceLength(int ticks);
        int     totalRaceTicks;
        float   raceLengthMeters;
        
        float   mRollerDiameterMm;
        bool    bHardwareConnected;
        bool    bUseKph = true;
    };
}

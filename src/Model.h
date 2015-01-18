//
//  Model.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Utilities.h"

#include "PlayerData.h"

namespace gfx{

    class Model {
      public:
        static Model* getInstance();
        void setRaceLengthMeters( float meters );
        int getRaceLengthTicks(){ return totalRaceTicks; }
        
        int                             startTimeMillis;
        int                             elapsedRaceTimeMillis;
        
        void setNumRacers( int num ){ mNumRacers = num; }
        int getNumRacers(){ return mNumRacers; }
        
        void resetPlayers();
        
        std::string                     rollerDiameterMm;
        std::vector<gfx::PlayerData*>   playerData;
        
        bool                            bHardwareConnected;
        
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
    };
}
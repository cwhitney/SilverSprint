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
    enum RACE_STATE {
        RACE_IDLE = 0,
        RACE_RUNNING,
        RACE_FINISHED
    };
    
    class Model {
      public:
        static Model* getInstance();
        void setRaceLengthMeters( float meters );
        int getRaceLengthTicks(){ return totalRaceTicks; }
        
        int                             startTimeMillis;
        int                             elapsedRaceTimeMillis;
        
        void setNumRacers( int num ){ mNumRacers = num; }
        int getNumRacers(){ return fmax(1, fmin(mNumRacers, 4)); }
        
        std::string                     rollerDiameterMm;
        std::vector<gfx::PlayerData*>   playerData;
        
        RACE_STATE                      raceState;
        
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
//
//  Model.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

#include "PlayerData.h"

namespace gfx{
    class Model {
      public:
        static Model* getInstance();
        void setRaceLength(int ticks);
        
        int                             elapsedRaceTimeMillis;
        std::vector<gfx::PlayerData*>   playerData;
        
      private:
        // PRIVATE CONSTRUCTOR + COPY
        Model();
        ~Model();
        Model(Model const&){};
        
        static Model    *mInstance;
        int             totalRaceTicks;
    };
}
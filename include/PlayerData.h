//
//  PlayerData.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

namespace gfx {

    class PlayerData {
    public:
        PlayerData();
        
        double getMph(){
            return mph;
        }
        
        double getKph(){
            return mph * 1.60934;
        }
        
        
        std::string player_name;
        double pctComplete;
        double mph;
        double dist;
        double millis;
    };

}
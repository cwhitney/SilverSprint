//
//  PlayerData.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/CinderMath.h"

namespace gfx {
    class PlayerData {
    public:
        PlayerData() :
            bFinishedRace(false),
            curRaceTicks(0),
            finishTimeMillis(0),
            pctComplete(0.0),
            mph(0),
            totalRaceTicks(500)
        {}
        
        void reset(){
            bFinishedRace = false;
            curRaceTicks = 0;
            finishTimeMillis = 0;
            pctComplete = 0.0;
            mph = 0;
        }
        
        bool didFinishRace(){
            return bFinishedRace;
        }
        
        double getMph(){
            
            
            return mph;
        }
        
        double getKph(){
            return getMph() * 1.60934;
        }
        
        double getPercent(){
            double racePct = ci::math<double>::clamp((double)curRaceTicks / (double)totalRaceTicks, 0.0, 1.0);
            return racePct;
        }
        
        double getDistance(){
            double wheelDiameter = 672.0; // millimeters
            double oneTickMeters = wheelDiameter * M_PI / 1000.0;
            
            if( curRaceTicks > totalRaceTicks ){
                curRaceTicks = totalRaceTicks;
            }
            
            double distMeters = oneTickMeters * curRaceTicks;
            
            return distMeters;
        }
        
        void setFinished( int finalTimeMillis ){
            bFinishedRace = true;
            finishTimeMillis = finalTimeMillis;
        }
        
        std::string player_name;
        int curRaceTicks;
        int totalRaceTicks;
        
      private:
        
        bool        bFinishedRace;
        double      finishTimeMillis;
        double      pctComplete;
        double      mph;
    };

}
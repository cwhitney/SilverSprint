//
//  PlayerData.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/CinderMath.h"

namespace gfx {
    class PlayerData {
    public:
        PlayerData() :
            bFinishedRace(false),
            curRaceTicks(0),
            lastRaceTicks(0),
            finishTimeMillis(0),
            pctComplete(0.0),
            mph(0),
            maxMph(0),
            totalRaceTicks(500)
        {}
        
        void reset(){
            bFinishedRace = false;
            curRaceTicks = 0;
            lastRaceTicks = 0;
            finishTimeMillis = 0;
            pctComplete = 0.0;
            mph = 0;
            maxMph = 0;
        }
        
        bool didFinishRace(){
            return bFinishedRace;
        }
        
        double getMph(){ return mph; }
        double getKph(){ return getMph() * 1.60934; }
        double getMaxMph(){ return maxMph; }
        
        double getPercent(){
            double racePct = ci::math<double>::clamp((double)curRaceTicks / (double)totalRaceTicks, 0.0, 1.0);
            return racePct;
        }
        
        double getDistance(){
            double oneTickMeters = rollerCircumfMm / 1000.0;
            
            if( curRaceTicks > totalRaceTicks ){
                curRaceTicks = totalRaceTicks;
            }
            
            double distMeters = oneTickMeters * curRaceTicks;
            
            return distMeters;
        }
        
        bool isFinished(){
            return bFinishedRace;
        }
        
        void setFinished( const int &finalTimeMillis ){
            bFinishedRace = true;
            finishTimeMillis = finalTimeMillis;
        }
        
        void setRollerDiameter( float diameterMm ){
            rollerCircumfMm = diameterMm * M_PI;
        }
        
        void updateRaceTicks( const int &numTicks )
        {
            lastRaceTicks = curRaceTicks;
            curRaceTicks = numTicks;
        }
        
        void updateRaceTicks( const int &numTicks, const int &millisDt )
        {
            lastRaceTicks = curRaceTicks;
            curRaceTicks = numTicks;
            
            float dist = (curRaceTicks - lastRaceTicks) * rollerCircumfMm;
            float kph = dist * millisDt / 1000.0;
            mph = kph * 0.621371;
            if( mph > maxMph ) maxMph = mph;
        }
        
        std::string player_name;
        int         totalRaceTicks;
        double      finishTimeMillis;
        
      private:
        bool        bFinishedRace;
        double      pctComplete;
        float       mph, maxMph;
        float       rollerCircumfMm;
        int         curRaceTicks, lastRaceTicks;
    };

}
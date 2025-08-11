//
//  PlayerData.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include <string>
#include <limits>

#include "cinder/app/App.h"
#include "cinder/CinderMath.h"
#include "cinder/Log.h"

#include "tools/CircBuffer.h"
#include "tools/CheapCircBuffer.h"

namespace gfx {
    typedef std::shared_ptr<class PlayerData> PlayerDataRef;
    
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
            totalRaceTicks(500),
            mSpeedBuffer( std::make_shared<CircBuffer>(60) ),
            rollerCircumfMm(0)
        {
            // mMphBuffer.reset(0.0);
        }
        
        void reset(){
            bFinishedRace = false;
            curRaceTicks = 0;
            lastRaceTicks = 0;
            finishTimeMillis = 0;
            pctComplete = 0.0;
            mph = 0;
            maxMph = 0;
            mLastRaceTimeMs = 0;
            
            mSpeedBuffer->reset();
            mMphBuffer.reset(0.0);
        }
        
        bool didFinishRace(){
            return bFinishedRace;
        }
        
        double getMph() const { return mph; }
        double getKph() const { return getMph() * 1.60934; }
        double getMaxMph() const { return maxMph; }
        double getMaxKph() const { return maxMph * 1.60934; }
        
        double getPercent() const {
            double racePct = ci::math<double>::clamp((double)curRaceTicks / (double)totalRaceTicks, 0.0, 1.0);
            return racePct;
        }
        
        double getDistance() const {
            double oneTickMeters = rollerCircumfMm / 1000.0;
            double distMeters = oneTickMeters * curRaceTicks;
            
            return distMeters;
        }
        
        double getDistanceMeters() const
        {
            return getDistance();
        }
        
        double getDistanceFeet() const
        {
            return getDistance() * 3.28084;
        }
        
        bool isFinished() const 
        {
            return bFinishedRace;
        }
        
        void setFinished( const int &finalTimeMillis, const int &finalRaceTicks )
        {
            bFinishedRace = true;
            finishTimeMillis = finalTimeMillis;
            
            lastRaceTicks = curRaceTicks;
            curRaceTicks = finalRaceTicks;
            
            float dist = getDistance();
            float avgSpeed =  dist / finishTimeMillis * 3600.0;
            
            // 8.7412587413 m/s = 31.46km/hr
            
            CI_LOG_I("Finished " << finalRaceTicks << " :: " << avgSpeed);
        }
        
        void setRollerDiameter( float diameterMm ){
            rollerCircumfMm = diameterMm * M_PI;
        }
        
        const int& getCurrentRaceTicks()
        {
            return curRaceTicks;
        }
        
        void updateRaceTicks(const int &numTicks, const int &curRaceMillis)
        {
            //            if(numTicks == curRaceTicks)
            //                return;
            
            lastRaceTicks = curRaceTicks;
            curRaceTicks = numTicks;
            int dtMillis = curRaceMillis - mLastRaceTimeMs;
            int dtTicks = curRaceTicks - lastRaceTicks;
            mLastRaceTimeMs = curRaceMillis;
            
            /*
             {
             mSpeedBuffer->add({curRaceMillis, dtTicks});
             double avg = mSpeedBuffer->getAverageTicksSec();
             double metersPerSec = avg * rollerCircumfMm / 1000.0;
             double kph = metersPerSec * 3.6; // 3.6 m/sec = 1 km/hr
             mph = kph * 0.621371;
             if( mph > maxMph ) maxMph = mph;
             }
             //*/
            
            //*
            // if it's not the first update
            if(dtMillis > 0){
                // you moved this many meters in dtMillis amt of time
                double metersMoved = dtTicks * rollerCircumfMm / 1000.0;
                double secsElapsed = (dtMillis / 1000.0);
                double kph = (metersMoved / secsElapsed) * 3.6; // 3.6 m/sec = 1 km/hr
                
                mph = kph * 0.621371;
                
                mMphBuffer.push(mph);
                mph = mMphBuffer.getAverage();
                
                if( mph > maxMph ) maxMph = mph;
            }
            //*/
        }
        
        std::string player_name;
        int         totalRaceTicks;
        double      finishTimeMillis;
        ci::Color   playerColor;
        
    private:
        // These represent two different strats for determining average speed
        std::shared_ptr<CircBuffer>     mSpeedBuffer;
        CheapCircBuffer<double>         mMphBuffer;
        
        bool        bFinishedRace;
        double      pctComplete;
        float       mph, maxMph;
        float       rollerCircumfMm;
        int         curRaceTicks, lastRaceTicks;
        int         mLastRaceTimeMs = 0;
    };
    
}

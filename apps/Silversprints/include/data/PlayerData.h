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
        {
            mSpeedBuffer = std::make_shared<CircBuffer>(60);
            mMphBuffer.reset(0.0);
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
        
        double getMph(){ return mph; }
        double getKph(){ return getMph() * 1.60934; }
        double getMaxMph(){ return maxMph; }
        double getMaxKph(){ return maxMph * 1.60934; }
        
        double getPercent(){
            double racePct = ci::math<double>::clamp((double)curRaceTicks / (double)totalRaceTicks, 0.0, 1.0);
            return racePct;
        }
        
        double getDistance(){
            double oneTickMeters = rollerCircumfMm / 1000.0;
            double distMeters = oneTickMeters * curRaceTicks;
            
            return distMeters;
        }
        
        double getDistanceMeters()
        {
            return getDistance();
        }
        
        double getDistanceFeet()
        {
            return getDistance() * 3.28084;
        }
        
        bool isFinished()
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
            float avgSpeed =  dist / finishTimeMillis * 3600;
            
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
        struct CircBuffer {
        public:
            CircBuffer(const int &size){
                mDataVec.resize(size);
                for( auto d : mDataVec){
                    d = {0, 0};
                }
            }
            
            void reset()
            {
                for( int i=0; i<mDataVec.size(); i++){
                    mDataVec[i] = {0.0, 0.0};
                }
                
                bFirstRun = true;
            }
            
            //! Args are a pair, where first = the current time in milliseconds, second = the number of ticks since that last update
            void add( const std::pair<int, int> &timeAndTicks ){
                if(bFirstRun){
                    for( auto d : mDataVec){
                        d = {timeAndTicks.first, 0};
                    }
                    bFirstRun = false;
                }
                
                mDataVec[dex] = timeAndTicks;
                if(++dex >= mDataVec.size())
                    dex = 0;
            }
            
            //! Returns the average number of ticks per second in the recorded time period.
            double getAverageTicksSec()
            {
                int ld = (dex - 1 < 0) ? int(mDataVec.size()) - 1 : dex - 1;
                int early = (ld + 1) >= mDataVec.size() ? 0 : ld + 1;
                int earliestTimeMs = mDataVec[early].first;
                int latestTimeMs = mDataVec[ld].first;
                double dtSecs = (latestTimeMs - earliestTimeMs) / 1000.0;
                
                float totalTicks = 0.0;
                for( auto d : mDataVec){
                    if(d.second > 0.0){
                        totalTicks += d.second;
                    }
                }
                return totalTicks / dtSecs;
            }
            
        protected:
            int dex = 0;
            bool bFirstRun = true;
            std::vector< std::pair<int, int> > mDataVec;
        };
        
        // ----------------------------------------------------------------------
        template<typename T>
        struct CheapCircBuffer {
            CheapCircBuffer(const int &size = 60){
                mSize = size;
                mData.resize(mSize);
            }
            
            void reset(const T &defaultVal){
                for( auto d : mData){
                    d = defaultVal;
                }
            }
            
            void push(const T &val ){
                mData[dex] = val;
                if(++dex >= mSize) dex = 0;
            }
            
            
            const T& getAverage(){
                T t;
                for( auto d : mData){
                    t += d;
                }
                return t / (double)mSize;
            }
        private:
            int dex = 0;
            int mSize = 30;
            
            std::vector<T> mData;
        };
        
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

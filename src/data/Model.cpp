//
//  Model.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "data/Model.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

Model* Model::mInstance = NULL;

Model::Model(){
    elapsedRaceTimeMillis = 0;
    raceLengthMeters = 50;
    mRollerDiameterMm = 82.55; // This is 3.25 inches in mm
    totalRaceTicks = 0;
    startTimeMillis = 0;
    mNumRacers = 2;
    bHardwareConnected = false;
    
    playerColors[0] = ci::ColorA(185.0 / 255.0, 33.0 / 255.0, 64.0 / 255.0, 1.0);
    playerColors[1] = ci::ColorA(28.0 / 255.0, 145.0 / 255.0, 133.0 / 255.0, 1.0);
    playerColors[2] = ci::ColorA(22.0 / 255.0, 146.0 / 255.0, 84.0 / 255.0, 1.0);
    playerColors[3] = ci::ColorA(225.0 / 255.0, 185.0 / 255.0, 9.0 / 255.0, 1.0);
    
    for( int i=0; i<4; i++){
        playerData.push_back( new PlayerData() );
        playerData.back()->playerColor = playerColors[i];
    }
    
    playerData[0]->player_name = "Player 1";
    playerData[1]->player_name = "Player 2";
    playerData[2]->player_name = "Player 3";
    playerData[3]->player_name = "Player 4";
}

Model::~Model(){
    while( playerData.size() ){
        PlayerData *pd = playerData.back();
        playerData.pop_back();
        delete pd;
    }
}

Model * Model::getInstance(){
    if (!mInstance){
        mInstance = new Model();
    }
    return mInstance;
}

// --------------------------------------------------------------------

void Model::setRollerDiameterMm(const float &mm)
{
    mRollerDiameterMm = mm;
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->setRollerDiameter(mm);
    }
}

void Model::setRaceLengthMeters(const float &meters){
    raceLengthMeters = meters;
    
    float totalDistMm = meters * 1000.0;
    float oneRollerRev = mRollerDiameterMm * M_PI;
    int numTicks =  floor(totalDistMm / oneRollerRev);
    setRaceLength( numTicks );
}

void Model::setRaceLength(int ticks) {
    totalRaceTicks = ticks;
    
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->totalRaceTicks = totalRaceTicks;
    }
}

void Model::resetPlayers() {
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->reset();
    }
}

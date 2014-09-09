//
//  Model.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "Model.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

Model* Model::mInstance = NULL;

Model::Model(){
    elapsedRaceTimeMillis = 0;
    totalRaceTicks = 500;
    
    for( int i=0; i<4; i++){
        playerData.push_back( new PlayerData() );
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

void Model::setRaceLength(int ticks) {
    console() << "Set race ticks " << ticks << endl;
    totalRaceTicks = ticks;
    
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->totalRaceTicks = totalRaceTicks;
    }
}

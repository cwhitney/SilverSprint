//
//  RaceText.h
//  SilverSprints
//
//  Created by Charlie Whitney on 10/13/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "tools/Utils.h"

#include "data/PlayerData.h"
#include "data/Model.h"

namespace gfx{
    
    class RaceText {
    public:
        RaceText( const ci::Color &color );
        void draw( gfx::PlayerDataRef data, const ci::vec2 &offset );
        
    private:
        std::string toDec( const float &num, const int &decPlaces );
        
        ci::Color mColor;
        double  mLastSpeedUpdate = 0.0;
        double  mLastSpeed = 0.0;
    };
}

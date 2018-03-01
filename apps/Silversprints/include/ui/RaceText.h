//
//  RaceText.h
//  GoldsprintsFX
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
        void draw( PlayerData *data, ci::vec2 offset );
        
      private:
        std::string toDec( float num, int decPlaces );
        
        ci::Color mColor;
    };
}

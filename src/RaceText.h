//
//  RaceText.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/13/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "PlayerData.h"
#include "GFXGlobal.h"
#include "Model.h"

namespace gfx{

    class RaceText {
      public:
        RaceText( const ci::Color &color );
        void draw( gfx::PlayerData *data, ci::Vec2f offset );
        
      private:
        std::string toDec( float num, int decPlaces );
        
        ci::Color mColor;
        gfx::GFXGlobal  *mGlobal;
        Model           *mModel;
    };
}
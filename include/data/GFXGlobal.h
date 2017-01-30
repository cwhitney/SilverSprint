//
//  GFXGlobal.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

namespace gfx {

    enum RACE_TYPE {
        RACE_TYPE_TIME = 0,
        RACE_TYPE_DISTANCE
    };
    
    class GFXGlobal {
    public:
        static GFXGlobal* getInstance();
        
        ci::Font uiFont;
        ci::gl::TextureFontRef      texFont, winnerTexFont;
        ci::gl::TextureFontRef      countdownFont;
        
		ci::Color playerColors[4];
        
        ci::vec2   localToGlobal( ci::vec2 pos );
        void setScale( const ci::Rectf &scaledRect );
        
        std::string toTimestamp( int millis );
        std::string toTimestampPrecise( int millis );
        
        RACE_TYPE currentRaceType;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        GFXGlobal(){};
        GFXGlobal(GFXGlobal const&){};
        static GFXGlobal		*mInstance;
        
		void setup();
        ci::vec2 mScreenScale, mScreenOffset;
    };
        
}
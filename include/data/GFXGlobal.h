//
//  GFXGlobal.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/TextureFont.h"

namespace gfx {

    enum RACE_TYPE {
        RACE_TYPE_TIME = 0,
        RACE_TYPE_DISTANCE
    };
    
    class GFXGlobal {
    public:
        static GFXGlobal* getInstance();
        
        ci::Font uiFont;
        ci::gl::TextureFontRef      texFont;
        ci::gl::TextureFontRef      countdownFont;
        
        ci::Color playerColors[4] = {
            ci::ColorA( 185.0/255.0, 33.0/255.0, 64.0/255.0, 1.0 ),
            ci::ColorA( 28.0/255.0, 145.0/255.0, 133.0/255.0, 1.0 ),
            ci::ColorA( 22.0/255.0, 146.0/255.0, 84.0/255.0, 1.0 ),
            ci::ColorA( 225.0/255.0, 185.0/255.0, 9.0/255.0, 1.0 )
        };
        
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
        
        ci::vec2 mScreenScale, mScreenOffset;
    };
        
}
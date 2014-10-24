//
//  GFXGlobal.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
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
        
        ci::Color playerColors[4] = {
            ci::Color::hex(0xb92140),
            ci::Color::hex(0x1c9185),
            ci::Color::hex(0x169254),
            ci::Color::hex(0xe1b909)
        };
        
        
        ci::Vec2f   localToGlobal( ci::Vec2f pos );
        void setScale( const ci::Rectf &scaledRect );
        
        RACE_TYPE currentRaceType;
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        GFXGlobal(){};
        GFXGlobal(GFXGlobal const&){};
        static GFXGlobal		*mInstance;
        
        ci::Vec2f mScreenScale, mScreenOffset;
    };
        
}
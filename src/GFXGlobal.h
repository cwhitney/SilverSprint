//
//  GFXGlobal.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

namespace gfx {

    class GFXGlobal {
    public:
        static GFXGlobal* getInstance();
        
        
    private:
        // PRIVATE CONSTRUCTOR + COPY
        GFXGlobal(){};
        GFXGlobal(GFXGlobal const&){};
        static GFXGlobal		*mInstance;
    };
        
}
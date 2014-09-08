//
//  StagingView.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"


#include "StateManager.h"

namespace gfx{

    class StagingView {
      public:
        StagingView();
        void setup();
        void update();
        void draw();
        
        void start();
        void stop();
        
      private:
        void onStateChange(GFX_STATE newState);
        
        ci::gl::TextureRef  mBg;
        StateManager        *mStateManager;
        
        BOOL        bVisible;
    };

}
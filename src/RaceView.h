//
//  RaceView.h
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
#include "Model.h"

namespace gfx{

    class RaceView {
      public:
        RaceView();
        void setup();
        void update();
        void draw();
        
      private:
        void onStateChange(GFX_STATE newState);
            
        ci::gl::TextureRef  mBg;
        StateManager        *mStateManager;
        Model               *mModel;
        
        bool    bVisible;
    };

}
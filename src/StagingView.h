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

#include "PretzelGui.h"
#include "StateManager.h"
#include "Model.h"

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
        Model               *mModel;
        
        Pretzel::PretzelGui          *gui;
        
        BOOL        bVisible;
    };

}
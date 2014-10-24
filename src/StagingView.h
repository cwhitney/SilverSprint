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
#include "GFXGlobal.h"

#include "CiTextField.h"

namespace gfx{

    class StagingView {
      public:
        StagingView();
        void setup();
        void update();
        void draw();
        
        void start();
        void stop();
        
        void animateIn();
        void animateOut();
        
      private:
        void onStateChange(GFX_STATE newState);
        
        std::vector<CiTextField*>   mPlayerNames;
        
        ci::gl::TextureRef  mBg;
        StateManager        *mStateManager;
        Model               *mModel;
        gfx::GFXGlobal      *mGlobal;
        
        Pretzel::PretzelGui          *gui;
        
        BOOL        bVisible;
    };

}
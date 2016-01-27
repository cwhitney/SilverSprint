//
//  RaceView.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"

#include "StateManager.h"
#include "Model.h"
#include "RaceText.h"
#include "GFXGlobal.h"
#include "StartStopButton.h"
#include "CountDownGfx.h"
#include "WinnerModal.h"

namespace gfx {

    using RaceViewRef = std::shared_ptr<class RaceView>;
    
    class RaceView
    {
      public:
        RaceView();
        void setup();
        void update();
        void draw();
        
        void reloadShader();
        
      private:
        void onStateChange( APP_STATE newState );
        void onStartClicked();
        void onStopClicked();
        
        
        void setupVbos();
        ci::gl::VboMeshRef createVbo( float innerRad, float outerRad );
        
        ci::gl::TextureRef  mBg, mDial, mLogo;
        ci::vec2            mDialCenter;
        StateManager        *mStateManager;
        Model               *mModel;
        gfx::GFXGlobal      *mGlobal;
        
        std::vector<RaceText*>  mRaceTexts;
        bool    bVisible;
        
        StartStopButton         mStartStop;
        CountDownGfxRef         mCountDown;
        WinnerModalRef          mWinnerModal;
        
        ci::gl::TextureFontRef  mTimerFont;
        
        // center graphics
        std::vector<ci::gl::VboMeshRef> mVboList;
        ci::gl::GlslProgRef     mProgressShader;
    };
}
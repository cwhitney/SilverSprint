//
//  RaceView.h
//  SilverSprints
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

#include "tools/Utils.h"

#include "data/StateManager.h"
#include "data/Model.h"
#include "ui/RaceText.h"
#include "ui/StartStopButton.h"
#include "ui/CountDownGfx.h"
#include "ui/WinnerModal.h"

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

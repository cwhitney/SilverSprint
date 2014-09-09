//
//  GFXMain.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/app/KeyEvent.h"

#include "Model.h"
#include "SerialReader.h"
#include "StateManager.h"

#include "RaceView.h"
#include "StagingView.h"

namespace gfx{

    class GFXMain {
      public:
        GFXMain();
        ~GFXMain();
        void setup();
        void update();
        void draw();
        
      private:
        RaceView        *mRaceView;
        StagingView     *mStagingView;
        
        Model           *mModel;
        SerialReader    *mSerialReader;
        StateManager    *mStateManager;
        
        void onKeyDown( ci::app::KeyEvent event );
    };

}
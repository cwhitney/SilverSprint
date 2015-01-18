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
#include "GFXGlobal.h"
#include "SerialReader.h"
#include "StateManager.h"

#include "NavBarView.h"
#include "RaceView.h"
#include "StagingView.h"
#include "SettingsView.h"

namespace gfx{

    class GFXMain {
      public:
        GFXMain();
        ~GFXMain();
        void setup();
        void update();
        void draw( const ci::Rectf &drawRect );
        
        void resetPlayerData();
        
      private:
        void onStateChaged();
        void onRaceStateChanged();
        
        GFXGlobal       *mGlobal;
        
        NavBarView      *mNav;
        RaceView        *mRaceView;
        StagingView     *mStagingView;
        SettingsView    *mSettingsView;
        
        Model           *mModel;
        SerialReader    *mSerialReader;
        StateManager    *mStateManager;
        
        void onKeyDown( ci::app::KeyEvent event );
    };

}
//
//  GFXMain.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/app/KeyEvent.h"

#include "data/Model.h"
#include "data/SerialReader.h"
#include "data/StateManager.h"

#include "views/NavBarView.h"
#include "views/RaceView.h"
#include "views/RosterView.h"
#include "views/SettingsView.h"

#include "data/CsvLogger.h"

namespace gfx{
    
    using GFXMainRef = std::shared_ptr<class GFXMain>;

    class GFXMain
    {
      public:
        GFXMain();
        ~GFXMain();
        void setup();
        void update();
        void draw( const ci::Rectf &drawRect );
        void reloadShaders();
        
        void resetPlayerData();
        
      private:
        void onAppStateChaged( APP_STATE as );
        void onRaceStateChanged( RACE_STATE rc );
        
        void onRaceFinished();
        
        NavBarViewRef       mNav;
        RaceViewRef         mRaceView;
        RosterViewRef       mRosterView;
        SettingsViewRef     mSettingsView;
        
        SerialReaderRef     mSerialReader;
        StateManager        *mStateManager;
        
        void onKeyDown( ci::app::KeyEvent event );
    };

}

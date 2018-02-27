//
//  RosterView.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"

#include "data/StateManager.h"
#include "data/Model.h"
#include "data/GFXGlobal.h"

#include "ui/CiTextField.h"

namespace gfx{

    using RosterViewRef = std::shared_ptr<class RosterView>;
    
    class RosterView {
      public:
        RosterView();
        void setup();
        void update();
        void draw();
        
        void start();
        void stop();
        
        void animateIn();
        void animateOut();
        
      private:
        void onKeyDown( ci::app::KeyEvent event );
        void onMouseUp( ci::app::MouseEvent event );
        void onStateChange(APP_STATE newState);
        
        std::vector<CiTextField*>   mPlayerNames;
        ci::gl::TextureFontRef      tFont;
        
        std::vector<ci::Rectf>      mCancelRects;
        
        ci::gl::TextureRef  mBg, mCancelBtn;
        StateManager        *mStateManager;
        GFXGlobal           *mGlobal;
        
        
        BOOL        bVisible;
    };

}

//
//  SettingsView.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/15/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "data/StateManager.h"
#include "data/Model.h"
#include "data/GFXGlobal.h"
#include "ui/CiTextField.h"
#include "ui/NumStepper.h"
#include "ui/ThickLine.h"
#include "ui/ToggleBtn.h"

namespace gfx {
    
    using SettingsViewRef = std::shared_ptr<class SettingsView>;
    
    class SettingsView {
    public:
        SettingsView();
        void update();
        void draw();
        
      private:
        void onStateChange( APP_STATE newState );
        void onStepperPlusClick();
        void onStepperMinusClick();
        void onMouseUp( ci::app::MouseEvent event );
        
        CiTextField* makeSetting(ci::Rectf rect, std::string label, std::string txt);
        
        StateManager    *mStateManager;
        Model           *mModel;
        GFXGlobal       *mGlobal;
        
        bool            bVisible;
        
        ci::gl::TextureRef  mBg;
        ci::gl::TextureFontRef      tFont;
        
        CiTextField*        mTxtDiameter;
        CiTextField*        mTxtDistance;
        CiTextField*        mTxtNumRacers;
        
        NumStepper      mStepperPlus;
        NumStepper      mStepperMinus;
        
        ToggleBtnRef    mMphKphToggle;
        
        struct TextLabel {
            TextLabel(ci::vec2 pos_, std::string txt_ ){ pos=pos_; txt=txt_; }
            ci::vec2 pos;
            std::string txt;
        };
        std::vector<TextLabel>  mLabels;
        
        ThickLineRef    mXLine1, mXLine2, mCheckLine1, mCheckLine2;
        
        ci::Rectf   mConnectionRect;
    };
}

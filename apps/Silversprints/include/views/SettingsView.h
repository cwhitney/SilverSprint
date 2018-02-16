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

#include "Version.h"

#include "data/StateManager.h"
#include "data/Model.h"

#include "ui/CheckBox.h"
#include "ui/CiTextField.h"
#include "ui/NumStepper.h"
#include "ui/ThickLine.h"
#include "ui/ToggleBtn.h"

namespace gfx {
    
    using SettingsViewRef = std::shared_ptr<class SettingsView>;
    
    class SettingsView {
    public:
        SettingsView();
        ~SettingsView();
        void update();
        void draw();
        
      private:
        void onStateChange(APP_STATE newState, APP_STATE lastState);
        void onStepperPlusClick();
        void onStepperMinusClick();
        void onMouseUp( ci::app::MouseEvent event );
        
        CiTextField* makeSetting(ci::Rectf rect, std::string label, std::string txt);
        CiTextField* makeTextField(ci::Rectf rect, std::string txt);
        
        const int       mMaxRiders = 4;
        
        StateManager    *mStateManager;        
        bool            bVisible = false;
        
        ci::gl::TextureRef          mBg;
        ci::gl::TextureFontRef      tFont;
        
        CiTextField*        mTxtDiameter;
        CiTextField*        mTxtDistance;
        CiTextField*        mTxtTime;
        CiTextField*        mTxtNumRacers;
        
        NumStepper      mStepperPlus;
        NumStepper      mStepperMinus;
        
        CheckBoxRef     mConnectionBox;
        CheckBoxRef     mDistanceCheck;
        CheckBoxRef     mTimeTrialBox;
        CheckBoxRef     mRaceLoggingBox;

        ToggleBtnRef    mMphKphToggle;
        
        struct TextLabel {
            TextLabel(ci::vec2 pos_, std::string txt_ ){ pos=pos_; txt=txt_; }
            ci::vec2 pos;
            std::string txt;
        };
        std::vector<TextLabel>  mLabels;
        TextLabel   *mDistanceLabel, *mTimeLabel;
        
        ThickLineRef    mXLine1, mXLine2, mCheckLine1, mCheckLine2;
    };
}

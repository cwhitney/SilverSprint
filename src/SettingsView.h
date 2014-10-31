//
//  SettingsView.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/15/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"

#include "StateManager.h"
#include "Model.h"
#include "GFXGlobal.h"
#include "CiTextField.h"

namespace gfx {
    
class SettingsView {
public:
    SettingsView();
    void update();
    void draw();
    
  private:
    void onStateChange(GFX_STATE newState);
    void onMouseUp( ci::app::MouseEvent event );
    
    CiTextField* makeSetting(ci::Rectf rect, std::string label, std::string txt);
    
    StateManager    *mStateManager;
    Model           *mModel;
    GFXGlobal       *mGlobal;
    
    bool            bVisible;
    
    ci::gl::TextureRef  mBg;
    std::vector<CiTextField*>   mTextFieldList;
    ci::gl::TextureFontRef      tFont;
    
    struct TextLabel {
        TextLabel(ci::Vec2f pos_, std::string txt_ ){ pos=pos_; txt=txt_; }
        ci::Vec2f pos;
        std::string txt;
    };
    std::vector<TextLabel>  mLabels;
    
    ci::Rectf   mConnectionRect;
};

}
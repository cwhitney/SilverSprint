#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#include "data/StateManager.h"
#include "data/GFXGlobal.h"
#include "data/Model.h"
#include "data/PlayerData.h"

using WinnerModalRef = std::shared_ptr<class WinnerModal>;

class WinnerModal {
    
  public:
    static WinnerModalRef create(){ return WinnerModalRef( new WinnerModal() ); }
    
    void update();
    void draw();
    
  private:
    WinnerModal();
    void getWinners();
    std::string toString(float num, int precision);
    bool    bVisible;
    
    ci::Rectf       mCloseRect;
    gfx::GFXGlobal  *mGlobal;
    gfx::Model      *mModel;
    
    ci::gl::TextureRef  mWinnerGraphic;
    ci::Anim<float>     mAlpha;
    
    std::vector<gfx::PlayerData*>   mWinnersSorted;
};
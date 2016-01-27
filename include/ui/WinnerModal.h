#pragma once

#include "cinder/app/App.h"

#include "StateManager.h"
#include "GFXGlobal.h"

using WinnerModalRef = std::shared_ptr<class WinnerModal>;

class WinnerModal {
    
  public:
    static WinnerModalRef create(){ return WinnerModalRef( new WinnerModal() ); }
    
    void update();
    void draw();
    
  private:
    WinnerModal();
    bool    bVisible;
    
    ci::Rectf       mCloseRect;
    gfx::GFXGlobal  *mGlobal;
};
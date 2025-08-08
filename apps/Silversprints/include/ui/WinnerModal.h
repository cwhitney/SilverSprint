#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/CinderMath.h"
#include "cinder/Signals.h"
#include "cinder/Camera.h"
#include "tools/Utils.h"

#include "data/StateManager.h"
#include "data/Model.h"
#include "data/PlayerData.h"

#include "ParticleSystem.h"

using WinnerModalRef = std::shared_ptr<class WinnerModal>;

class WinnerModal 
{    
public:
    static WinnerModalRef create(){ return WinnerModalRef( new WinnerModal() ); }
    
    void update();
    void draw();

private:
    WinnerModal();
    void getWinners();
    std::string toString(float num, int precision);
    
    bool                bVisible = false;
    
    ci::Rectf           mWinnerRect;
    ci::gl::TextureRef  mWinnerGraphic;
    ci::Anim<float>     mAlpha = 0.0;
    
	ci::signals::Connection			mConn;
    std::vector<gfx::PlayerDataRef>   mWinnersSorted;
    
    sharkbox::ParticleSystemRef     mParticles;
    ci::CameraOrtho                 mCamOrtho;
    double lt = 0;
};

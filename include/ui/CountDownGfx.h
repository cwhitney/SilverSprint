//
//  CountDownGfx.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 3/17/15.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Timeline.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "StateManager.h"

namespace gfx {

    using CountDownGfxRef = std::shared_ptr<class CountDownGfx>;
    
    class CountDownGfx {
      public:
        static CountDownGfxRef create(){ return CountDownGfxRef(new CountDownGfx()); }
        void draw();
        
      private:
        CountDownGfx();
        void onRaceStateChange( RACE_STATE newState );
        
        StateManager        *mStateManager;
        RACE_STATE          mCurRaceState;
        
        std::string         mTxt;
        ci::Anim<float>     mAnimScale;
        ci::Anim<float>     mAnim3, mAnim2, mAnim1, mAnimGo;
        
        
        ci::gl::TextureRef mCount3Tex, mCount2Tex, mCount1Tex, mCountGoTex;
    };

}

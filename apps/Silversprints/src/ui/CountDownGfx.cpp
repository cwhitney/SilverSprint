//
//  CountDownGfx.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 3/17/15.
//
//

#include "ui/CountDownGfx.h"
#include "data/GFXGlobal.h"

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

CountDownGfx::CountDownGfx() :
    mAnim3(0.0),
    mAnim2(0.0),
    mAnim1(0.0),
    mAnimGo(0.0)
{
    mStateManager = StateManager::getInstance();
    mStateManager->signalOnRaceStateChange.connect( std::bind(&CountDownGfx::onRaceStateChange, this, std::placeholders::_1) );
    
    mTxt = "";
    
    mCount3Tex  = gl::Texture::create(loadImage(loadAsset("img/cd_03.png")));
    mCount2Tex  = gl::Texture::create(loadImage(loadAsset("img/cd_02.png")));
    mCount1Tex  = gl::Texture::create(loadImage(loadAsset("img/cd_01.png")));
    mCountGoTex = gl::Texture::create(loadImage(loadAsset("img/cd_go.png")));
}

void CountDownGfx::onRaceStateChange( RACE_STATE newState )
{
    if( newState == RACE_COUNTDOWN_3 ){
        timeline().apply( &mAnim3, 0.0f, 1.0f, 0.5f, EaseOutCubic());
        timeline().appendTo( &mAnim3, 1.0f, 0.0f, 0.3f, EaseOutCubic()).delay(0.2);
        
        timeline().apply(&mAnimScale, 0.0f, 1.0f, 0.3, EaseOutCubic());
        timeline().appendTo(&mAnimScale, 1.0f, 2.0f, 0.3, EaseOutCubic()).delay(0.4);
        mTxt = "3";
    }else if( newState == RACE_COUNTDOWN_2 ){
        timeline().apply( &mAnim2, 0.0f, 1.0f, 0.5f, EaseOutCubic());
        timeline().appendTo( &mAnim2, 1.0f, 0.0f, 0.3f, EaseOutCubic()).delay(0.2);
        
        timeline().apply(&mAnimScale, 0.0f, 1.0f, 0.3, EaseOutCubic());
        timeline().appendTo(&mAnimScale, 1.0f, 2.0f, 0.3, EaseOutCubic()).delay(0.4);
        mTxt = "2";
    }else if( newState == RACE_COUNTDOWN_1 ){
        timeline().apply( &mAnim1, 0.0f, 1.0f, 0.5f, EaseOutCubic());
        timeline().appendTo( &mAnim1, 1.0f, 0.0f, 0.3f, EaseOutCubic()).delay(0.2);
        
        timeline().apply(&mAnimScale, 0.0f, 1.0f, 0.3, EaseOutCubic());
        timeline().appendTo(&mAnimScale, 1.0f, 2.0f, 0.3, EaseOutCubic()).delay(0.4);
        mTxt = "1";
    }else if( newState == RACE_COUNTDOWN_GO ){
        timeline().apply( &mAnimGo, 0.0f, 1.0f, 0.5f, EaseOutCubic());
        timeline().appendTo( &mAnimGo, 1.0f, 0.0f, 0.3f, EaseOutCubic()).delay(0.2);
        
        timeline().apply(&mAnimScale, 0.0f, 1.0f, 0.3, EaseOutCubic());
        timeline().appendTo(&mAnimScale, 1.0f, 2.0f, 0.3, EaseOutCubic()).delay(0.4);
        mTxt = "GO";
    }
    
    else if( newState == RACE_STOPPED  ){
        mTxt = "";
    }
}

void CountDownGfx::draw() {
    if( mTxt != ""){
        gl::color( Color::white() );
//        vec2 txtSize = GFXGlobal::getInstance()->countdownFont->measureString( mTxt );
        
        gl::ScopedMatrices scMat;
        gl::translate(vec2(1920 / 2, 612));
        gl::scale(mAnimScale, mAnimScale);
        
        gl::ScopedColor sc3(1,1,1,mAnim3);
        gl::draw(mCount3Tex, (vec2)mCount3Tex->getSize() * vec2(-0.5) );
        
        gl::ScopedColor sc2(1,1,1,mAnim2);
        gl::draw(mCount2Tex, (vec2)mCount2Tex->getSize() * vec2(-0.5) );
        
        gl::ScopedColor sc1(1,1,1,mAnim1);
        gl::draw(mCount1Tex, (vec2)mCount2Tex->getSize() * vec2(-0.5) );
        
        gl::ScopedColor scg(1,1,1,mAnimGo);
        gl::draw(mCountGoTex, (vec2)mCountGoTex->getSize() * vec2(-0.5) );
        
//        GFXGlobal::getInstance()->countdownFont->drawString(mTxt, vec2(1920 / 2, 1080 / 2) + (txtSize * vec2(-0.5, 0.5) ) );
    }
}


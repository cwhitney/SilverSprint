#include "WinnerModal.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

WinnerModal::WinnerModal() :
    bVisible(false)
{
    mGlobal = GFXGlobal::getInstance();
    mCloseRect = Rectf(100,100,500,500);
    
    mWinnerGraphic = gl::Texture::create( loadImage( loadAsset("img/WinnerModal.png") ));
    mCloseRect = mWinnerGraphic->getBounds();
    mCloseRect.offset( (vec2(1920, 1080) - (vec2)mWinnerGraphic->getSize()) * vec2(0.5) );
    
    StateManager::getInstance()->signalOnRaceStateChange.connect( [&](RACE_STATE newState){
        if( newState == RACE_STATE::RACE_COMPLETE ){
            timeline().apply( &mAlpha, 0.0f, 1.0f, 1.0f, EaseOutQuad());
            bVisible = true;
        }else{
            bVisible = false;
            timeline().apply( &mAlpha, 1.0f, 0.0f, 1.0f, EaseOutQuad());
        }
    });
    
    getWindow()->getSignalMouseUp().connect( [&](MouseEvent event){
        if( mCloseRect.contains( mGlobal->localToGlobal(event.getPos())) ){
            StateManager::getInstance()->changeRaceState( RACE_STOPPED );
        }
    });
}

void WinnerModal::update()
{
    
}

void WinnerModal::draw()
{
    if( bVisible ){
        gl::ScopedColor(1,1,1,mAlpha);
        gl::draw( mWinnerGraphic, mCloseRect);
    }
}
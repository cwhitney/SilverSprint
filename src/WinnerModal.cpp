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
    
    StateManager::getInstance()->signalOnRaceStateChange.connect( [&](RACE_STATE newState){
        if( newState == RACE_STATE::RACE_COMPLETE ){
            bVisible = true;
        }else{
            bVisible = false;
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
        gl::drawSolidRect( mCloseRect );
    }
}
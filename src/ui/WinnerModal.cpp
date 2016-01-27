#include "WinnerModal.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

WinnerModal::WinnerModal() :
    bVisible(false)
{
    mModel = Model::getInstance();
    mGlobal = GFXGlobal::getInstance();
    mCloseRect = Rectf(100,100,500,500);
    
    mWinnerGraphic = gl::Texture::create( loadImage( loadAsset("img/WinnerModal.png") ));
    mCloseRect = mWinnerGraphic->getBounds();
    mCloseRect.offset( (vec2(1920, 1080) - (vec2)mWinnerGraphic->getSize()) * vec2(0.5) );
    
    StateManager::getInstance()->signalOnRaceStateChange.connect( [&](RACE_STATE newState){
        if( newState == RACE_STATE::RACE_COMPLETE ){
            getWinners();
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
    
    mAlpha = 0;
}

void WinnerModal::getWinners()
{
    mWinnersSorted.clear();
    
    for( int i=0; i<mModel->getNumRacers(); i++){
        mWinnersSorted.push_back( Model::getInstance()->getPlayerData(i) );
    }
    
    std::sort( mWinnersSorted.begin(), mWinnersSorted.end(), []( PlayerData* a, PlayerData *b) {
        return a->finishTimeMillis < b->finishTimeMillis;
    });
    
//    for( int i=0; i<mWinnersSorted.size(); i++){
//        console() << " finish :: " << mWinnersSorted[i]->player_name << " - " <<mWinnersSorted[i]->finishTimeMillis << endl;
//    }
}

void WinnerModal::update()
{
    
}

void WinnerModal::draw()
{
    if( bVisible ){
        gl::ScopedColor(1,1,1,mAlpha);
        
        gl::draw( mWinnerGraphic, mCloseRect);
        float ww = mGlobal->winnerTexFont->measureString(mWinnersSorted[0]->player_name).x;
        {
            gl::ScopedMatrices scMat;
            gl::translate( mCloseRect.getUpperLeft() );
            gl::color(0,0,0,mAlpha);
            mGlobal->winnerTexFont->drawString( mWinnersSorted[0]->player_name, vec2(550 - ww*0.5, 288));
            gl::color(1,1,1,mAlpha);
            mGlobal->texFont->drawString( mGlobal->toTimestampPrecise(mWinnersSorted[0]->finishTimeMillis), vec2(356, 362));
            mGlobal->texFont->drawString( toString(mWinnersSorted[0]->getMaxMph(), 1) + "mph", vec2(604-20, 362));
        }
        {
            gl::ScopedMatrices scMat;
            gl::translate( mCloseRect.getUpperLeft() );
            gl::translate( vec2(20, 688) );
            for( int i=1; i<mModel->getNumRacers(); i++){
                mGlobal->texFont->drawString( mWinnersSorted[i]->player_name, vec2(0, 42));
                mGlobal->texFont->drawString( mGlobal->toTimestampPrecise(mWinnersSorted[i]->finishTimeMillis), vec2(0, 82));
                mGlobal->texFont->drawString( toString(mWinnersSorted[i]->getMaxMph(), 1) + "mph", vec2(190, 82));
                
                gl::translate( vec2(384, 0) );
            }
        }
        
    }
}

std::string WinnerModal::toString(float num, int precision)
{
    stringstream stream;
    stream << fixed << setprecision(precision) << num;
    return stream.str();
}



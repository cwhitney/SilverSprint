#include "ui/WinnerModal.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

WinnerModal::WinnerModal() :
    bVisible(false)
{
    mModel = Model::getInstance();
    mGlobal = GFXGlobal::getInstance();
//    mWinnerRect = Rectf(100,100,500,500);
    
    mWinnerGraphic = gl::Texture::create( loadImage( loadAsset("img/WinnerModal.png") ));
    mWinnerRect = mWinnerGraphic->getBounds();
    mWinnerRect.offset( (vec2(1920, 1080) - (vec2)mWinnerGraphic->getSize()) * vec2(0.5) );
    
    StateManager::getInstance()->signalOnRaceStateChange.connect( [&](RACE_STATE newState){
        if( newState == RACE_STATE::RACE_COMPLETE ){
            getWinners();
            timeline().apply( &mAlpha, 0.0f, 1.0f, 1.0f, EaseOutQuad());
            bVisible = true;

			mConn = getWindow()->getSignalMouseUp().connect([&](MouseEvent event) {
				//if (mWinnerRect.contains(mGlobal->localToGlobal(event.getPos()))) {
					StateManager::getInstance()->changeRaceState(RACE_STOPPED);
				//}
			});
        }else{
            bVisible = false;
            timeline().apply( &mAlpha, 1.0f, 0.0f, 1.0f, EaseOutQuad());
			mConn.disconnect();
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
        gl::ScopedColor scBg(0,0,0,mAlpha * 0.5);
        gl::drawSolidRect( Rectf(0,0,1920,1080) );
        
        gl::ScopedColor scA(1,1,1,mAlpha);
        gl::draw( mWinnerGraphic, mWinnerRect);
        float ww = mGlobal->winnerTexFont->measureString(mWinnersSorted[0]->player_name).x;
        {
            gl::ScopedMatrices scMat;
            gl::translate( mWinnerRect.getUpperLeft() );
            gl::color(0,0,0,mAlpha);
            mGlobal->winnerTexFont->drawString( mWinnersSorted[0]->player_name, vec2(550 - ww*0.5, 288));
            gl::color(1,1,1,mAlpha);
            mGlobal->texFont->drawString( mGlobal->toTimestampPrecise(mWinnersSorted[0]->finishTimeMillis), vec2(356, 362));
            mGlobal->texFont->drawString( toString(mWinnersSorted[0]->getMaxMph(), 1) + "mph", vec2(604-20, 362));
        }
        {
            gl::ScopedMatrices scMat;
            gl::translate( mWinnerRect.getUpperLeft() );
            gl::translate( vec2(0, 688) );
            
            int NR = (mModel->getNumRacers()-1);
            float gap = 24;
            float totalWidth = (360 * NR) + gap * (NR-1);
            Rectf bgRect(0,0,360,100);
            Rectf botRect(0,110,360,114);
            
            for( int i=0; i<NR; i++){
                gl::ScopedMatrices scMat2;
                
                
                float lm = (totalWidth * -0.5) + (360 + gap) * i;
                float offsetX = lm + mWinnerRect.getWidth() * 0.5;

                gl::translate(vec2(offsetX, 0));
                {
                    gl::ScopedColor scCol( mWinnersSorted[i+1]->playerColor );
                    gl::drawSolidRect( bgRect );
                    
                    gl::ScopedColor scB(Color::black());
                    gl::drawSolidRect(Rectf(193, 60, 197, 88));
                    
                    gl::ScopedColor scW(Color::gray(229));
                    gl::drawSolidRect( botRect );
                }
                
                mGlobal->texFont->drawString( mWinnersSorted[i+1]->player_name, vec2(20, 42));
                mGlobal->texFont->drawString( mGlobal->toTimestampPrecise(mWinnersSorted[i+1]->finishTimeMillis), vec2(20, 82));
                mGlobal->texFont->drawString( toString(mWinnersSorted[i+1]->getMaxMph(), 1) + "mph", vec2(210, 82));
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



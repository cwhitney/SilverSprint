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
    
    mCamOrtho = CameraOrtho(0, 1920, 0, 1080, -100, 100);
    
    mParticles = std::make_shared<sharkbox::ParticleSystem>();
    mParticles->emitterPos = vec3(1920*0.5, 1080 + 30, 0);
    mParticles->emitterSize = vec3(1920,0,0);
    mParticles->emitVel = 4;
    mParticles->emitVelRand = 0.3;
    mParticles->emitDir = vec3(0,-1,0);
    mParticles->emitRatePerSec = 30.0;
    
    mParticles->mParticleSize = vec3(25);
    mParticles->particleRotAngle = vec3(1,1,1);
    mParticles->particleRotSpeed = 0.1;
    mParticles->particleLifespan = 10.0;
    
    StateManager::getInstance()->signalOnRaceStateChange.connect( [&](RACE_STATE newState){
        if( newState == RACE_STATE::RACE_COMPLETE ){
            getWinners();
            timeline().apply( &mAlpha, 0.0f, 1.0f, 1.0f, EaseOutQuad());
            bVisible = true;
            mParticles->resetSystem();

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
    
    if(GFXGlobal::getInstance()->currentRaceType == RACE_TYPE_DISTANCE){
        std::sort( mWinnersSorted.begin(), mWinnersSorted.end(), []( PlayerData* a, PlayerData *b) {
            return a->finishTimeMillis < b->finishTimeMillis;
        });
    }else{
        std::sort( mWinnersSorted.begin(), mWinnersSorted.end(), []( PlayerData* a, PlayerData *b) {
            return a->getCurrentRaceTicks() < b->getCurrentRaceTicks();
        });
    }
    
//    for( int i=0; i<mWinnersSorted.size(); i++){
//        console() << " finish :: " << mWinnersSorted[i]->player_name << " - " <<mWinnersSorted[i]->finishTimeMillis << endl;
//    }
}

void WinnerModal::update()
{
    double ct = getElapsedSeconds();
    double dt = ct - lt;
    mParticles->update(dt);
    
    lt = ct;
}

void WinnerModal::draw()
{
    if( bVisible ){
        gl::ScopedColor scBg(0,0,0,mAlpha * 0.5);
        gl::drawSolidRect( Rectf(0,0,1920,1080) );
        
        {// Particles
            gl::ScopedMatrices scOrtho;
            gl::ScopedDepth scD(true);
            gl::setMatrices(mCamOrtho);
            gl::ScopedColor scWin( mWinnersSorted[0]->playerColor );
            mParticles->draw();
        }
        
        gl::ScopedColor scA(1,1,1,mAlpha);
        gl::draw( mWinnerGraphic, mWinnerRect);
        float ww = mGlobal->winnerTexFont->measureString(mWinnersSorted[0]->player_name).x;
        // draw the main winner
        {
            gl::ScopedMatrices scMat;
            gl::translate( mWinnerRect.getUpperLeft() );
            gl::color(0,0,0,mAlpha);
            mGlobal->winnerTexFont->drawString( mWinnersSorted[0]->player_name, vec2(550 - ww*0.5, 288));
            
            // draw the correct race metric, time or distance
            if(mGlobal->currentRaceType == RACE_TYPE_TIME){
                gl::ScopedColor scCol(ColorA(0,0,0,mAlpha));
                string winLabel = "TIME";
                vec2 labelSize = mGlobal->winnerUiFont->measureString(winLabel);
                mGlobal->winnerUiFont->drawString(winLabel, vec2(441, 319) - vec2(labelSize.x*0.5, 0));
                
                gl::color(1,1,1,mAlpha);
                mGlobal->texFont->drawString( mGlobal->toTimestampPrecise(mWinnersSorted[0]->finishTimeMillis), vec2(356, 362));
            }else{
                gl::ScopedColor scCol(ColorA(0,0,0,mAlpha));
                string winLabel = "DISTANCE";
                vec2 labelSize = mGlobal->winnerUiFont->measureString(winLabel);
                mGlobal->winnerUiFont->drawString(winLabel, vec2(441, 319) - vec2(labelSize.x*0.5, 0));
                
                gl::color(1,1,1,mAlpha);
                mGlobal->texFont->drawString(to_string(mWinnersSorted[0]->getDistanceMeters()) + "m", vec2(356, 362));
            }
            

            gl::color(1,1,1,mAlpha);
            mGlobal->texFont->drawString( mGlobal->toTimestampPrecise(mWinnersSorted[0]->finishTimeMillis), vec2(356, 362));
            if(mModel->getUsesKph()){
                mGlobal->texFont->drawString( toString(mWinnersSorted[0]->getMaxKph(), 1) + "kph", vec2(604-20, 362));
            }else{
                mGlobal->texFont->drawString( toString(mWinnersSorted[0]->getMaxMph(), 1) + "mph", vec2(604-20, 362));
            }
        }
        // Draw the individual boxes below
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
                if(mModel->getUsesKph()){
                    mGlobal->texFont->drawString( toString(mWinnersSorted[i+1]->getMaxKph(), 1) + "kph", vec2(210, 82));
                }else{
                    mGlobal->texFont->drawString( toString(mWinnersSorted[i+1]->getMaxMph(), 1) + "mph", vec2(210, 82));
                }
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



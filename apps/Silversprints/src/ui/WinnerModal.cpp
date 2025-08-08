#include "ui/WinnerModal.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

WinnerModal::WinnerModal() :
    bVisible(false)
{
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
    
    StateManager::instance().signalOnRaceStateChange.connect( [&](RACE_STATE newState){
        if( newState == RACE_STATE::RACE_COMPLETE ){
            getWinners();
            timeline().apply( &mAlpha, 0.0f, 1.0f, 1.0f, EaseOutQuad());
            bVisible = true;
            mParticles->resetSystem();

			mConn = getWindow()->getSignalMouseUp().connect([&](MouseEvent event) {
				//if (mWinnerRect.contains(Model::instance().localToGlobal(event.getPos()))) {
					StateManager::instance().changeRaceState(RACE_STOPPED);
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
    
    for( int i=0; i<Model::instance().getNumRacers(); i++){
        PlayerDataRef pd = Model::instance().getPlayerData(i);
        if (pd != nullptr){
            mWinnersSorted.push_back( pd );
        }
    }
    
    // DISTANCE
    if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_DISTANCE){
        std::sort( mWinnersSorted.begin(), mWinnersSorted.end(), []( PlayerDataRef a, PlayerDataRef b) {
            return a->finishTimeMillis < b->finishTimeMillis;
        });
    }
    // RACE_TYPE_TIME
    else{
        std::sort( mWinnersSorted.begin(), mWinnersSorted.end(), []( PlayerDataRef a, PlayerDataRef b) {
            return a->getCurrentRaceTicks() > b->getCurrentRaceTicks();
        });
    }
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
        gl::ScopedColor scBg(0,0,0, mAlpha * 0.5f);
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
        float ww = Model::instance().winnerTexFont->measureString(mWinnersSorted[0]->player_name).x / Model::instance().getFontScale();
        // draw the main winner
        {
            gl::ScopedMatrices scMat;
            gl::translate( mWinnerRect.getUpperLeft() );
            gl::color(0,0,0,mAlpha);
            Model::instance().winnerTexFont->drawString( mWinnersSorted[0]->player_name, vec2(550 - ww*0.5, 288), Model::instance().getTfDrawOpts());
            
            // draw the correct race metric, time or distance
            if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_DISTANCE){
                gl::ScopedColor scCol(ColorA(0,0,0,mAlpha));
                string winLabel = "TIME";
                vec2 labelSize = Model::instance().winnerUiFont->measureString(winLabel) / Model::instance().getFontScale();
                Model::instance().winnerUiFont->drawString(winLabel, vec2(441, 319) - vec2(labelSize.x*0.5, 0), Model::instance().getTfDrawOpts());
                
                gl::color(1,1,1,mAlpha);
                Model::instance().texFont->drawString( sb::utils::millisToTimestamp(mWinnersSorted[0]->finishTimeMillis), vec2(356, 362), Model::instance().getTfDrawOpts());
            }
            // RACE_TYPE_TIME
            else{
                gl::ScopedColor scCol(ColorA(0,0,0,mAlpha));
                string winLabel = "DISTANCE";
                vec2 labelSize = Model::instance().winnerUiFont->measureString(winLabel) / Model::instance().getFontScale();
                Model::instance().winnerUiFont->drawString(winLabel, vec2(441, 319) - vec2(labelSize.x*0.5, 0), Model::instance().getTfDrawOpts());
                
                gl::color(1,1,1,mAlpha);
                if(Model::instance().getUsesKph()){
                    Model::instance().texFont->drawString(toString(mWinnersSorted[0]->getDistanceMeters(), 2) + "m", vec2(356, 362), Model::instance().getTfDrawOpts());
                }else{
                    Model::instance().texFont->drawString(toString(mWinnersSorted[0]->getDistanceFeet(), 2) + "ft", vec2(356, 362), Model::instance().getTfDrawOpts());
                }
            }
            
            gl::color(1,1,1,mAlpha);
            if(Model::instance().getUsesKph()){
                Model::instance().texFont->drawString( toString(mWinnersSorted[0]->getMaxKph(), 1) + "kph", vec2(604-20, 362), Model::instance().getTfDrawOpts());
            }else{
                Model::instance().texFont->drawString( toString(mWinnersSorted[0]->getMaxMph(), 1) + "mph", vec2(604-20, 362), Model::instance().getTfDrawOpts());
            }
        }
        // Draw the individual boxes below
        {
            gl::ScopedMatrices scMat;
            gl::translate( mWinnerRect.getUpperLeft() );
            gl::translate( vec2(0, 688) );
            
            int NR = (Model::instance().getNumRacers()-1);
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
                
                Model::instance().texFont->drawString( mWinnersSorted[i+1]->player_name, vec2(20, 42), Model::instance().getTfDrawOpts());
                
                gl::color(1,1,1,mAlpha);
                // draw finish stats
                if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_DISTANCE){
                    Model::instance().texFont->drawString( sb::utils::millisToTimestamp(mWinnersSorted[i+1]->finishTimeMillis), vec2(20, 82), Model::instance().getTfDrawOpts());
                }
                // RACE_TYPE_TIME
                else{
                    if(Model::instance().getUsesKph()){
                        Model::instance().texFont->drawString(toString(mWinnersSorted[i+1]->getDistanceMeters(), 2) + "m", vec2(20, 82), Model::instance().getTfDrawOpts());
                    }else{
                        Model::instance().texFont->drawString(toString(mWinnersSorted[i+1]->getDistanceFeet(), 2) + "ft", vec2(20, 82), Model::instance().getTfDrawOpts());
                    }
                }
                
                // draw top speed
                if(Model::instance().getUsesKph()){
                    Model::instance().texFont->drawString( toString(mWinnersSorted[i+1]->getMaxKph(), 1) + "kph", vec2(210, 82), Model::instance().getTfDrawOpts());
                }else{
                    Model::instance().texFont->drawString( toString(mWinnersSorted[i+1]->getMaxMph(), 1) + "mph", vec2(210, 82), Model::instance().getTfDrawOpts());
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



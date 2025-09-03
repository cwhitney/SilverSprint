//
//  Model.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "data/Model.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

Model::Model(){
    playerColors[0] = ci::ColorA(185.0 / 255.0, 33.0 / 255.0, 64.0 / 255.0, 1.0);
    playerColors[1] = ci::ColorA(28.0 / 255.0, 145.0 / 255.0, 133.0 / 255.0, 1.0);
    playerColors[2] = ci::ColorA(22.0 / 255.0, 146.0 / 255.0, 84.0 / 255.0, 1.0);
    playerColors[3] = ci::ColorA(225.0 / 255.0, 185.0 / 255.0, 9.0 / 255.0, 1.0);

#if defined( CINDER_MSW )
    mFontScale = ci::app::getWindowContentScale();
	mDrawOpts = gl::TextureFont::DrawOptions().scale(1.0f / mFontScale).pixelSnap(false);
#else
    mFontScale = 1.0;
    mDrawOpts = gl::TextureFont::DrawOptions().scale(1.0f / mFontScale).pixelSnap(true);
#endif
    
    uiFont = ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 35.0 );
    texFont = gl::TextureFont::create(uiFont, gl::TextureFont::Format().enableMipmapping());
    
    winnerUiFont = gl::TextureFont::create(ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 18.0), gl::TextureFont::Format().enableMipmapping());
    winnerTexFont = gl::TextureFont::create(ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 70.0), gl::TextureFont::Format().enableMipmapping());
    countdownFont = gl::TextureFont::create(ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 235.0), gl::TextureFont::Format().enableMipmapping());
    
    for(int i=0; i<4; i++){
        playerData.push_back( std::make_shared<PlayerData>() );
        playerData.back()->playerColor = playerColors[i];
        playerData.back()->player_name = "Rider " + toString(i+1);
    }
}

Model::~Model()
{
    CI_LOG_I("Model destructor called");

    playerData.clear();
    mSerialDeviceList.clear();
}

// --------------------------------------------------------------------
void Model::setScale( const ci::Rectf &scaledRect )
{
    vec2 sc( (float)getWindowWidth() / 1920.0, (float)getWindowHeight() / 1080.0);
    mScreenScale = vec2( min(sc.x, sc.y) );

    mScreenOffset = scaledRect.getUpperLeft();
}

ci::vec2 Model::localToGlobal( ci::vec2 pos )
{
    return (vec2(pos - mScreenOffset) / mScreenScale);
}


void Model::setRollerDiameterMm(const float &mm)
{
    RaceSettings.mRollerDiameterMm = mm;
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->setRollerDiameter(mm);
    }
}

void Model::setRaceLengthMeters(const float &meters) {
    RaceSettings.raceLengthMeters = meters;
    
    float totalDistMm = meters * 1000.0;
    float oneRollerRev = RaceSettings.mRollerDiameterMm * M_PI;
    int numTicks =  floor(totalDistMm / oneRollerRev);
    setRaceLength( numTicks );
}

void Model::setRaceLength(int ticks) {
    RaceSettings.totalRaceTicks = ticks;
    
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->totalRaceTicks = RaceSettings.totalRaceTicks;
    }
}

void Model::resetPlayers() {
    for( int i=0; i<playerData.size(); i++){
        playerData[i]->reset();
    }
}

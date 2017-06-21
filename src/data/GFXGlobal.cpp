//
//  GFXGlobal.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#ifdef __linux
    //linux
    #include "../../include/data/GFXGlobal.h"
#else
    // Windows & OSX
    #include "data/GFXGlobal.h"
#endif

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

GFXGlobal* GFXGlobal::mInstance = NULL;

GFXGlobal * GFXGlobal::getInstance(){
    if (!mInstance){
        mInstance = new GFXGlobal();


		mInstance->setup();
    }
    return mInstance;
}

void GFXGlobal::setup()
{
	uiFont = ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 35);
	texFont = gl::TextureFont::create(mInstance->uiFont);

	winnerTexFont = gl::TextureFont::create(ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 70));
	countdownFont = gl::TextureFont::create(ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 235));

	currentRaceType = RACE_TYPE::RACE_TYPE_TIME;

	playerColors[0] = ci::ColorA(185.0 / 255.0, 33.0 / 255.0, 64.0 / 255.0, 1.0);
	playerColors[1] = ci::ColorA(28.0 / 255.0, 145.0 / 255.0, 133.0 / 255.0, 1.0);
	playerColors[2] = ci::ColorA(22.0 / 255.0, 146.0 / 255.0, 84.0 / 255.0, 1.0);
	playerColors[3] = ci::ColorA(225.0 / 255.0, 185.0 / 255.0, 9.0 / 255.0, 1.0);
}

void GFXGlobal::setScale( const ci::Rectf &scaledRect )
{
    vec2 sc( (float)getWindowWidth() / 1920.0, (float)getWindowHeight() / 1080.0);
    mScreenScale = vec2( min(sc.x, sc.y) );

    mScreenOffset = scaledRect.getUpperLeft();
}

ci::vec2 GFXGlobal::localToGlobal( ci::vec2 pos ){
    return (vec2(pos - mScreenOffset) / mScreenScale);
}

std::string addZ(int n) {
    return (n<10? "0":"") + to_string(n);
}

std::string addZ100(int n) {
    if( n>100 ) {
        return to_string(n);
    }else if(n>10) {
        return "0" + to_string(n);
    }
    return "00" + to_string(n);
}

std::string GFXGlobal::toTimestampPrecise( int millis ){
    //    millis = 755020;
    // millis = 12048;

    int ms = millis % 1000;
    millis = (millis - ms) / 1000;
    int secs = millis % 60;
    millis = (millis - secs) / 60;
    int mins = millis % 60;

    return addZ(mins) + ":" + addZ(secs) + ":" + addZ100(ms);
}

std::string GFXGlobal::toTimestamp( int millis ){
    //    millis = 755020;
    // millis = 12048;

    int ms = millis % 1000;
    millis = (millis - ms) / 1000;
    int secs = millis % 60;
    millis = (millis - secs) / 60;
    int mins = millis % 60;

    while( ms > 100 ){
        ms /= 10.0;
    }

    return addZ(mins) + ":" + addZ(secs) + ":" + addZ(ms);
}

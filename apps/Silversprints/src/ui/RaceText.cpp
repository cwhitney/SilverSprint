//
//  RaceText.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 10/13/14.
//
//

#include "ui/RaceText.h"
// #include <boost/algorithm/string.hpp>
#include <sstream>

#include "impl/win.h"

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

RaceText::RaceText( const ci::Color &color ){
    mColor = color;
}

std::string RaceText::toDec( const float &num, const int &decPlaces ) {
    std::ostringstream buffer;
    buffer << fixed << setprecision( decPlaces ) << num;
    return buffer.str();
}

void RaceText::draw( gfx::PlayerData *data, const ci::vec2 &offset )
{
    gl::pushMatrices();{
        gl::translate( offset );
        
        int w = 7;
        
        gl::color( mColor );
        // PLAYER NUMBER ---------
        
        
        // COLORED GRID ----------
        gl::translate( 150, 0);
        if( !data->didFinishRace() ){
            gl::drawSolidRect( Rectf(0,       0,      1670,   w) );   // T
            gl::drawSolidRect( Rectf(0,       0,      w,      68) );  // L
            gl::drawSolidRect( Rectf(1670-w,  0,      1670,   68) );  // R
            gl::drawSolidRect( Rectf(0,       68-w,   1670,   68) );  // B
            gl::drawSolidRect( Rectf(1462-w,  w,      1462, 68-w) );  // MID
        }else{
            gl::drawSolidRect( Rectf(0,       0,      1670,   68) );  // BG
            gl::color( Color::black() );
            gl::drawSolidRect( Rectf(1462-w,  w,      1462, 68-w) );  // MID
        }
        
        // TEXT ------------------
        gl::color( Color::white() );
        
        // NAME
        // boost::to_upper(data->player_name);
        // ci::toUpper(data->player_name);
        auto pn = data->player_name;
        std::transform(pn.begin(), pn.end(), pn.begin(), ::toupper);
        
        Model::instance().texFont->drawString(data->player_name, vec2(20, 44), Model::instance().getTfDrawOpts());
        
        // SPEED
        if( Model::instance().getUsesKph()){
            if(getElapsedSeconds() - mLastSpeedUpdate > Model::instance().speedUpdateInterval){
                mLastSpeed = data->getKph();
                mLastSpeedUpdate = getElapsedSeconds();
            }
			
			std::string tx = toDec(mLastSpeed, 1) + " KPH";
			vec2 strSize = Model::instance().texFont->measureString(tx) / Model::instance().getFontScale();
//            Model::instance().texFont->drawString(tx, vec2(1580 - strSize.x, 44), Model::instance().getTfDrawOpts() );
            Model::instance().texFont->drawString(tx, vec2(1430 - strSize.x, 44), Model::instance().getTfDrawOpts() );
        }else{
            if(getElapsedSeconds() - mLastSpeedUpdate > Model::instance().speedUpdateInterval){
                mLastSpeed = data->getMph();
                mLastSpeedUpdate = getElapsedSeconds();
            }

			std::string tx = toDec(mLastSpeed, 1) + " MPH";
			vec2 strSize = Model::instance().texFont->measureString(tx) / Model::instance().getFontScale();
			Model::instance().texFont->drawString(tx, vec2(1430 - strSize.x, 44), Model::instance().getTfDrawOpts() );
        }
        
        
        // DISTANCE AND TIME
        if( Model::instance().getCurrentRaceType() == Model::RACE_TYPE::RACE_TYPE_TIME ){
            if(Model::instance().getUsesKph()){
                Model::instance().texFont->drawString(toDec(data->getDistanceMeters(), 2) + "m", vec2(1485,44),
					Model::instance().getTfDrawOpts());
            }else{
                Model::instance().texFont->drawString(toDec(data->getDistanceFeet(), 2) + "ft", vec2(1485,44),
					Model::instance().getTfDrawOpts());
            }
        }
        else if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE::RACE_TYPE_DISTANCE){
            if( data->didFinishRace() ){
                Model::instance().texFont->drawString(sb::utils::millisToTimestamp( data->finishTimeMillis ), vec2(1485,44),
					Model::instance().getTfDrawOpts());
            }else{
                Model::instance().texFont->drawString(sb::utils::millisToTimestamp( Model::instance().elapsedRaceTimeMillis ), vec2(1485,44),
					Model::instance().getTfDrawOpts());
            }
        }
        
    }gl::popMatrices();
}

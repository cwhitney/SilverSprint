//
//  RaceText.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/13/14.
//
//

#include "ui/RaceText.h"
#include <boost/algorithm/string.hpp>
#include <sstream>

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

RaceText::RaceText( const ci::Color &color ){
    mColor = color;
}

std::string RaceText::toDec( float num, int decPlaces ) {
    std::ostringstream buffer;
    buffer << fixed << setprecision( decPlaces ) << num;
    return buffer.str();
}

void RaceText::draw( gfx::PlayerData *data, ci::vec2 offset )
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
        boost::to_upper(data->player_name);
        Model::instance().texFont->drawString(data->player_name, vec2(20, 44) );
        
        // sPEED
        if( Model::instance().getUsesKph()){
            gl::drawStringRight( toDec(data->getKph(), 0) + " KPH", vec2(1430,20), Color::white(), Model::instance().uiFont );
        }else{
            gl::drawStringRight( toDec(data->getMph(), 0) + " MPH", vec2(1430,20), Color::white(), Model::instance().uiFont );
        }
        
        if( Model::instance().getCurrentRaceType() == Model::RACE_TYPE::RACE_TYPE_TIME ){
            if(Model::instance().getUsesKph()){
                Model::instance().texFont->drawString(toDec(data->getDistanceMeters(), 2) + "m", vec2(1485,44));
            }else{
                Model::instance().texFont->drawString(toDec(data->getDistanceFeet(), 2) + "ft", vec2(1485,44));
            }
        }
        else if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE::RACE_TYPE_DISTANCE){
            if( data->didFinishRace() ){
                Model::instance().texFont->drawString(sb::utils::millisToTimestamp( data->finishTimeMillis ), vec2(1485,44) );
            }else{
                Model::instance().texFont->drawString(sb::utils::millisToTimestamp( Model::instance().elapsedRaceTimeMillis ), vec2(1485,44) );
            }
        }
        
    }gl::popMatrices();
}


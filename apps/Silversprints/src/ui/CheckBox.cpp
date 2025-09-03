//
//  CheckBox.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 15/11/17.
//
//

#include "ui/CheckBox.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

CheckBox::CheckBox(ci::vec2 position, bool interactive) :
    bInteractive(interactive)
{    
    BaseButton::set(position.x, position.y, position.x + 100, position.y + 100);
    
    mXLine1 = ThickLine::create( BaseButton::getUpperLeft()  + vec2(20,20),  BaseButton::getLowerRight() - vec2(20,20), 5);
    mXLine2 = ThickLine::create( BaseButton::getUpperRight() + vec2(-20,20), BaseButton::getLowerLeft() + vec2(20,-20), 5);
    
    vec2 p1(20,60), p2(40,80), p3(80,20);
    vec2 t = getUpperLeft();;
    mCheckLine1 = ThickLine::create( p1+t, p2+t + vec2(1.5,1.5), 5 );
    mCheckLine2 = ThickLine::create( p2+t, p3+t, 5 );
}

void CheckBox::onMouseOver()
{
}

void CheckBox::onMouseOut()
{
}

bool CheckBox::isChecked()
{
    return bChecked;
}

void CheckBox::onClick()
{
    if(bInteractive){
        bChecked = !bChecked;
        signalOnClick.emit();
    }
}

void CheckBox::draw()
{
    if(bInteractive){
        gl::ScopedColor scPc( Model::instance().playerColors[0] );
        gl::drawStrokedRect( (Rectf) *this, 5);
        
        if(bChecked){
            gl::drawSolidRect(*this);
            gl::ScopedColor scCol(Color::gray(0.1f));
            mCheckLine1->draw();
            mCheckLine2->draw();
        }else{
            mXLine1->draw();
            mXLine2->draw();
        }
        
    }else{
        gl::ScopedColor scPc( Model::instance().playerColors[0] );
        gl::drawSolidRect( (Rectf)*this );
        
        gl::ScopedColor scCol(Color::gray(0.1f));
        
        if(bChecked){
            mCheckLine1->draw();
            mCheckLine2->draw();
        }else{
            mXLine1->draw();
            mXLine2->draw();
        }
    }
}

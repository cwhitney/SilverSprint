//
//  CheckBox.cpp
//  GoldsprintsFX
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
    BaseButton(),
    bInteractive(interactive)
{
    
    mGlobal = GFXGlobal::getInstance();
    
    mBounds = Rectf(position, position + vec2(100));
    
    mXLine1 = ThickLine::create( mBounds.getUpperLeft()  + vec2(20,20),  mBounds.getLowerRight() - vec2(20,20), 5);
    mXLine2 = ThickLine::create( mBounds.getUpperRight() + vec2(-20,20), mBounds.getLowerLeft() + vec2(20,-20), 5);
    
    vec2 p1(20,60), p2(40,80), p3(80,20);
    vec2 t = mBounds.getUpperLeft();;
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
    if(bInteractive && !bChecked){
        bChecked = !bChecked;
        signalOnClick.emit();
    }
}

void CheckBox::draw()
{
    if(bInteractive){
        gl::ScopedColor scPc( mGlobal->playerColors[0] );
        gl::drawStrokedRect(mBounds, 5);
        
        if(bChecked){
            gl::drawSolidRect(mBounds);
            gl::ScopedColor scCol(Color::gray(0.1));
            mCheckLine1->draw();
            mCheckLine2->draw();
        }else{
            mXLine1->draw();
            mXLine2->draw();
        }
        
    }else{
        gl::ScopedColor scPc( mGlobal->playerColors[0] );
        gl::drawSolidRect( mBounds );
        
        gl::ScopedColor scCol(Color::gray(0.1));
        
        if(bChecked){
            mCheckLine1->draw();
            mCheckLine2->draw();
        }else{
            mXLine1->draw();
            mXLine2->draw();
        }
    }
}

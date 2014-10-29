//
//  CiTextField.cpp
//  CinderText
//
//  Created by Charlie Whitney on 10/13/14.
//
//

#include "CiTextField.h"

using namespace ci;
using namespace ci::app;
using namespace std;

CiTextField::~CiTextField(){
    mMouseDownCb.disconnect();
    mMouseDragCb.disconnect();
}

CiTextField::CiTextField( std::string text, ci::Rectf bounds, ci::Font font ){
    setText(text);
    setBounds(bounds);
    
    mColorStroke = Color(1,1,1);
    mColorFill = Color::gray(0.2);
    mColorText = Color(1,1,1);
    mColorHighlight = Color::hex(0xb6d6fd);
    
    mFont = font;
    
    if( mFont ){
        tFont = gl::TextureFont::create( mFont );
        emSize = tFont->measureString("M");
    }
    
    ci::app::WindowRef window = cinder::app::getWindow();
    mMouseDownCb = window->getSignalMouseDown().connect( std::bind(&CiTextField::onMouseDown, this, std::placeholders::_1) );
    mMouseDragCb = window->getSignalMouseDrag().connect( std::bind(&CiTextField::onMouseDrag, this, std::placeholders::_1) );
    mMouseUpCb   = window->getSignalMouseUp().connect( std::bind(&CiTextField::onMouseUp, this, std::placeholders::_1) );
    
    mKeyDownCb  = window->getSignalKeyDown().connect( std::bind(&CiTextField::onKeyDown, this, std::placeholders::_1) );
    mKeyUpCb    = window->getSignalKeyUp().connect( std::bind(&CiTextField::onKeyUp, this, std::placeholders::_1) );
    
    bDragging = false;
    bActive = false;
    bHighlighted = false;
    bEnabled = true;
    
    bUseScissorTest = true;
    
    mCaratIndex = mText.size();
}

void CiTextField::setBounds( const ci::Rectf &bounds ){
    mBounds = bounds;
}

void CiTextField::setText(std::string text){
    mText = text;
}

int CiTextField::eraseString( int start, int end ){
    int s = min(mCaratStart, mCaratIndex);
    int e = max(mCaratStart, mCaratIndex);
    
    mText.erase(s, e-s);
    return s;
}

void CiTextField::onKeyDown(KeyEvent event){
    if( !bActive ){
        return;
    }
    
    if( event.getCode() == KeyEvent::KEY_RETURN || event.getCode() == KeyEvent::KEY_ESCAPE ){
        blur();
        bHighlighted = false;
        return;
    }
    
    if( event.getCode() == KeyEvent::KEY_LEFT ){
        bDragging = false;
        mCaratIndex = max(mCaratIndex - 1, 0);
        
        bHighlighted = false;
    }else if( event.getCode() == KeyEvent::KEY_RIGHT ){
        bDragging = false;
        mCaratIndex = min(mCaratIndex + 1, (int)mText.size());
        
        bHighlighted = false;
    }
    
    else if( event.getCode() == KeyEvent::KEY_DELETE ){
        if( bHighlighted ){
            mCaratIndex = eraseString( mCaratStart, mCaratIndex);
        }else{
            mText.erase(mCaratIndex, 1);
        }
        
        bHighlighted = false;
    }
    
    else if( event.getCode() == KeyEvent::KEY_BACKSPACE ){
        if( mText.size() ){
            if( bHighlighted ){
                mCaratIndex = eraseString( mCaratStart, mCaratIndex);
            }else{
                mText.erase(mCaratIndex-1, 1);
                mCaratIndex--;
            }
            
            bHighlighted = false;
        }
    }
    
    else if (event.getCode() > 31 && event.getCode() < 272){  // delete is in here, but we handle it above
        if( bHighlighted ){
            int s = min(mCaratStart, mCaratIndex);
            int e = max(mCaratStart, mCaratIndex);
            
            mText.erase(s, e-s);
            mText.insert(s, 1, event.getChar() );
            
            mCaratIndex = s+1;
            
            bHighlighted = false;
        }else{
            mText.insert(mCaratIndex, 1, event.getChar() );
            ++mCaratIndex;
        }
        
        bHighlighted = false;
    }
    
    
}

void CiTextField::onKeyUp(KeyEvent event){
    
}

void CiTextField::onMouseDown( ci::app::MouseEvent event ){
    bHighlighted = false;
    bDragging = false;
    blur();
    
    Vec2f pos = gfx::GFXGlobal::getInstance()->localToGlobal(event.getPos());
    
    if( mBounds.contains(pos) ){
        bDragging = true;
        bActive = true;
        
        mCaratStart = mCaratIndex = getCursorIndex( pos - mBounds.getUpperLeft() ); // pass in the local pos
        mCaratIndex = mCaratStart;
    }
}
void CiTextField::onMouseDrag( ci::app::MouseEvent event ){
    Vec2f pos = gfx::GFXGlobal::getInstance()->localToGlobal(event.getPos());
    
    if( bDragging ){
        mCaratIndex = getCursorIndex( pos - mBounds.getUpperLeft() );
    }
}
void CiTextField::onMouseUp( ci::app::MouseEvent event ){
    Vec2f pos = gfx::GFXGlobal::getInstance()->localToGlobal(event.getPos());
    
    if( bDragging ){
        bActive = true;
        mCaratIndex = getCursorIndex( pos - mBounds.getUpperLeft() );
        if( mCaratStart != mCaratIndex ){
            bHighlighted = true;
        }
        
    }else{
        blur();
        bHighlighted = false;
    }
    
    bDragging = false;
}

void CiTextField::focus( bool selectAll ){
    bActive = true;
    
    if( selectAll ){
        mCaratStart = 0;
        mCaratIndex = mText.size();
        bHighlighted = true;
    }
}

void CiTextField::blur(){
    bActive = false;
    
    boost::trim(mText);
    
    if( mText !="" ){
        enable();
    }
}

int CiTextField::getCursorIndex( const ci::Vec2f &localPos ){
    Vec2f pos = localPos - padding;
    
    int i=0;
    for( ; i<mText.size()+1; i++){
        Vec2f sm = tFont->measureString( mText.substr(0,i) );
        
        if( sm.x > pos.x ){
            if(i>1){
                return i-1;
            }else{
                return 0;
            }
            break;
        }
    }
    // We didn't find it, so put the cursor at the end
    if( i == mText.size()+1 ){
        return mText.size();
    }
    
    return mCaratIndex;
}

void CiTextField::draw(){
    
    float alpha = 1.0;
    if( !bEnabled ){
        alpha = 0.2;
    }
    
    if( bActive ){
        mColorStroke.a = 1.0;
        
        mColorFill = ColorA(0,0,0,0);
        mColorText = mColorStroke;
    }else{
        mColorFill = mColorStroke;
        mColorText = ColorA(0,0,0);
        
        mColorFill.a = alpha;
        mColorText.a = alpha;
        mColorStroke.a = alpha;
    }
    
    if( !tFont ){
        if( !mFont )
            return;
        
        tFont = gl::TextureFont::create( mFont );
        emSize = tFont->measureString("M");
    }
    
    if( bUseScissorTest ){
        glEnable(GL_SCISSOR_TEST);
        glScissor( mBounds.x1, getWindowHeight() - mBounds.y1 - mBounds.getHeight(), mBounds.getWidth(), mBounds.getHeight() );
    }
    
    gl::color( mColorFill );
    gl::drawSolidRect( mBounds );
    
    gl::color( mColorStroke );
    gl::drawStrokedRect( mBounds );
    
    gl::pushMatrices();
    gl::translate( padding );
    
    // draw our cursor line
    if( bActive ){
        mCursorPos.x = tFont->measureString( mText.substr(0, mCaratIndex) ).x + mBounds.x1;
        
        gl::color( mColorHighlight );
        gl::pushMatrices();{
            gl::translate( 0, mBounds.y1 + emSize.y*0.1);
            gl::drawLine( mCursorPos, mCursorPos + Vec2f(0, emSize.y) );
        }gl::popMatrices();
    
        if( bDragging || bHighlighted ) {
            gl::color( mColorHighlight );
            
            Vec2f UL( tFont->measureString( mText.substr(0, mCaratStart) ).x, 0);
            Vec2f LR( tFont->measureString( mText.substr(0, mCaratIndex) ).x, emSize.y);
            Rectf highlighRect(UL, LR);
            highlighRect.offset( mBounds.getUpperLeft() + Vec2f(0, emSize.y *0.1) );
            gl::drawSolidRect( highlighRect );
        }
    }
    
    gl::color( mColorText );
    tFont->drawString(mText, Vec2f(mBounds.x1,mBounds.y1 + emSize.y) );
    
    gl::popMatrices();
    
    if( bUseScissorTest ){
        glDisable(GL_SCISSOR_TEST);
    }
}


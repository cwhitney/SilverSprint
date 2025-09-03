//
//  CiTextField.h
//  CinderText
//
//  Created by Charlie Whitney on 10/13/14.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Rect.h"
#include "cinder/gl/TextureFont.h"
#include "data/Model.h"

typedef std::shared_ptr<class CiTextField> CiTextFieldRef;

class CiTextField : public ci::Rectf {
  public:
    CiTextField( const std::string &text, const ci::Rectf &_bounds, ci::Font font );
    ~CiTextField();
    
    void setText( std::string text );
    std::string getText(){ return mText; }
    
    void setBounds( const ci::Rectf &bounds );
    const ci::Rectf& getBounds(){ return mBounds; }

    void focus( bool selectAll=false);   // Become active.
    void blur();    // Become inactive. (Same as unfocus)
    
    void enable(){ bEnabled = true; }
    void disable(){ bEnabled = false; }
    bool isEnabled(){ return bEnabled; }
    
    void draw();
    
    bool bUseScissorTest;
    bool isActive(){ return bActive; }
    
    bool visible;
    
    ci::ColorA      mColorStroke, mColorFill, mColorText, mColorHighlight;
    
    ci::vec2       padding;
    
  private:
    void onMouseDown( ci::app::MouseEvent event );
    void onMouseDrag( ci::app::MouseEvent event );
    void onMouseUp( ci::app::MouseEvent event );
    
    void onKeyDown( ci::app::KeyEvent event );
    void onKeyUp( ci::app::KeyEvent event );
    
	ci::vec2 measureStr(const std::string &str);
    int getCursorIndex( const ci::vec2 &pos );
    int eraseString( int start, int end );
    
    bool            bDragging;
    bool            bActive;
    bool            bHighlighted;
    
    ci::vec2        mCursorPos;
    int             mCaratIndex, mCaratStart;
    
    bool            bEnabled;
    
    std::string     mText;
    ci::Rectf       mBounds;
    ci::vec2        emSize;
	float			mFontScale = 1.0;
    
    ci::Font                    mFont;
    ci::gl::TextureFontRef      tFont;

	ci::gl::TextureFont::DrawOptions	mDrawOpts;
    
    // mouse events
    ci::signals::Connection       mMouseDownCb,
                                  mMouseDragCb,
                                  mMouseUpCb,
                                  mKeyDownCb,
                                  mKeyUpCb;
};

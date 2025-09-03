#pragma once

#include "cinder/app/App.h"
#include "cinder/Signals.h"
#include "cinder/Log.h"
#include "cinder/Rect.h"

#include "data/Model.h"
#include "ui/ThickLine.h"

using namespace std;

using DropDownRef = std::shared_ptr<class DropDown>;

class DropDown : public ci::Rectf {
  public:
	DropDown(const ci::Rectf &bounds, ci::gl::TextureFontRef font);
	
	template <typename T>
	void setOptions( const std::vector<T> &opts );
	
	void setSelected( const int &num );
	void setSelected( const std::string &value );
	
	void draw();
	bool visible = false;
	
	ci::signals::Signal<void(std::string)> signalOnOptionSelect;
	
  private:
	void onMouseDown( ci::app::MouseEvent event );
	void onMouseMove( ci::app::MouseEvent event );
	
	ci::signals::Connection     mMouseDownCb, mMouseMoveCb;
	
	ci::gl::TextureFontRef	mFont;
	vector<string>			mOptions;		// contains the list of options
	vector<string>			mOptionsDisplay;
	
	int         mSelectedIndex = 0;
	std::string mSelectedStr;
	bool        bOpen = false;
	
	ci::Rectf   mDrawerRect, mBgRect, mDropDownRect;
	int         mHighlight = -1;
	
	ThickLineRef    mDn1, mDn2;
};

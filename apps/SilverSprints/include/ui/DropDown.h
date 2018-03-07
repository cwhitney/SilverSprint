#pragma once

#include "cinder/app/App.h"
#include "cinder/Signals.h"

#include "data/Model.h"

using namespace std;

using DropDownRef = std::shared_ptr<class DropDown>;

class DropDown {
  public:
    DropDown(const ci::Rectf &bounds, ci::gl::TextureFontRef font);
    
    void setOptions( const std::vector<std::string> &opts );
    void setSelected( const int &num );
    const int& getSelected(){ return mSelected; }
    
    void draw();
    
  private:
    void onMouseDown( ci::app::MouseEvent event );
    void onMouseUp( ci::app::MouseEvent event );
    
    ci::signals::Connection     mMouseDownCb, mMouseUpCb;
    
    ci::gl::TextureFontRef  mFont;
    vector<string>          mOptions;
    ci::Rectf   mBounds;
    int         mSelected = 0;
    bool        bVisible = false;
    bool        bOpen = false;
};

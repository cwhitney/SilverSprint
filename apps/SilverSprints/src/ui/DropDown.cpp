#include "ui/DropDown.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

DropDown::DropDown( const ci::Rectf &bounds, ci::gl::TextureFontRef font) {
    mBounds = bounds;
    
    ci::app::WindowRef window = cinder::app::getWindow();
    mMouseDownCb = window->getSignalMouseDown().connect( std::bind(&DropDown::onMouseDown, this, std::placeholders::_1) );
    mMouseMoveCb = window->getSignalMouseMove().connect( std::bind(&DropDown::onMouseMove, this, std::placeholders::_1) );
    
    mFont = font;
}

template <>
void DropDown::setOptions(const std::vector<std::string> &opts )
{
    mOptions = opts;
    mOptionsRaw = opts;
    vec2 drawerSize(mBounds.getWidth(), min<float>(mBounds.getHeight() * 3.0, mBounds.getHeight() * opts.size()));
    mDrawerRect = Rectf(mBounds.getUpperLeft(), mBounds.getUpperLeft() + drawerSize);
}

template <>
void DropDown::setOptions(const std::vector<Model::SerialDeviceDesc> &opts )
{    
    mIndices.resize(opts.size());
    for(int i=0; i<opts.size(); i++){
        mIndices[i] = i;
    }
    
    std::vector<std::string> rawOpts;
    std::vector<std::string> displayOpts;
    for( const auto &opt : opts){
        std::string pn = opt.portName;
        std::string pd = opt.portDescription;
        
        if(mFont->measureString(pd).x > mBounds.getWidth() - 10){
            while( mFont->measureString(pd).x > mBounds.getWidth() - 10 ){
                pd = pd.substr(0, pd.size()-1);
            }
            pd = pd.substr(0, pd.size()-1);
            pd.append("…");                     // this might not show up in all fonts
        }
        
        pn.append("\n");
        pn.append(pd);
        
        displayOpts.push_back(pn);
        rawOpts.push_back(opt.portName);
    }
    
    setOptions(displayOpts);
    
    // calling the template above sets rawOpts=displayOpts, so we need to reset it here
    mOptionsRaw = rawOpts;
    
    CI_LOG_I("Setting new options. Attempting to find " << mSelectedStr);
    setSelected(mSelectedStr);
}

void DropDown::onMouseDown( ci::app::MouseEvent event ){
    if(!visible){
        return;
    }
    
    vec2 pos = Model::instance().localToGlobal(event.getPos());
    
    if(bOpen){
        if(mHighlight > -1){
            setSelected(mIndices[mHighlight]);
            mHighlight = -1;
        }
        bOpen = false;
    }else if( mBounds.contains(pos) && mOptions.size() > 1){
        bOpen = !bOpen;
    }
}

void DropDown::onMouseMove(ci::app::MouseEvent event){
    if(!visible){
        return;
    }
    
    if(bOpen){
        vec2 pos = Model::instance().localToGlobal(event.getPos());
        if(mDrawerRect.contains(pos)){
            mHighlight = (pos.y - mDrawerRect.y1) / mBounds.getHeight();
        }else{
            mHighlight = -1;
        }
    }
    
}

void DropDown::setSelected(const int &num) {
    signalOnOptionSelect.emit(mOptionsRaw[num]);
    
    mIndices[0] = num;
    int dex = 1;
    for(int i=0; i<mIndices.size(); i++){
        if(i==num)
            continue;
        
        mIndices[dex] = i;
        dex++;
    }
    mSelectedStr = mOptionsRaw[mIndices[0]];
}

void DropDown::setSelected(const std::string &value) {
    CI_LOG_I("DropDown attempt to set selected: " << value);
    // If a value matches one of the raw options, it will be first, otherwise the list will remain in original order
    std::sort(mIndices.begin(), mIndices.end(), [&](int a, int b){
        if(mOptionsRaw[a] == value){
            return true;
        }else if(mOptionsRaw[b] == value){
            return false;
        }else if(a < b){
            return true;
        }
        
        return false;
    });
}

void DropDown::draw() {
    if(!visible){
        return;
    }
    
    gl::ScopedColor scCol(Model::instance().playerColors[0]);
    
    if(!bOpen){
        gl::drawSolidRect( mBounds );
        
        if(mOptions.size() > 0){
            gl::ScopedColor scC(Color::black());
            mFont->drawString(mOptions[mIndices[0]], mBounds + vec2(10, 25));
        }
    }else{
        gl::drawSolidRect(mDrawerRect);
        
        gl::ScopedColor scC(Color::black());
        for( int i=0; i<mOptions.size(); i++){
            if(mHighlight == i){
                gl::ScopedColor scBg(Color::white());
                gl::drawSolidRect(Rectf(mBounds.x1, mBounds.y1 + mBounds.getHeight() * i, mBounds.x2, mBounds.y2 + (mBounds.getHeight()+1) * i));
            }
            
            mFont->drawString(mOptions[mIndices[i]], mBounds + vec2(10, 25 + i * mBounds.getHeight()));
        }
        
    }
}



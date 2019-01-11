#include "ui/DropDown.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

DropDown::DropDown( const ci::Rectf &bounds, ci::gl::TextureFontRef font) {
//    mBounds = bounds;
    Rectf::set(bounds.x1, bounds.y1, bounds.x2, bounds.y2);
    
    ci::app::WindowRef window = cinder::app::getWindow();
    mMouseDownCb = window->getSignalMouseDown().connect( std::bind(&DropDown::onMouseDown, this, std::placeholders::_1) );
    mMouseMoveCb = window->getSignalMouseMove().connect( std::bind(&DropDown::onMouseMove, this, std::placeholders::_1) );
    
    mFont = font;
    
    mBgRect = Rectf( getUpperLeft(), getLowerRight() - vec2(50,0));
    mDropDownRect = Rectf(mBgRect.getUpperRight() + vec2(5, 0), getLowerRight() );
    
    float ll = 10;
    mDn1 = ThickLine::create( mDropDownRect.getCenter() - vec2(ll, ll*0.5), mDropDownRect.getCenter() + vec2(2, ll*0.5), 5);
    mDn2 = ThickLine::create( mDropDownRect.getCenter() + vec2(ll, -ll*0.5), mDropDownRect.getCenter() + vec2(-1, ll*0.5), 5);
}

template <>
void DropDown::setOptions(const std::vector<std::string> &opts )
{
    mOptions = opts;
    mOptionsRaw = opts;
    vec2 drawerSize( getWidth(), min<float>( getHeight() * 3.0, getHeight() * opts.size()));
    mDrawerRect = Rectf( getUpperLeft(), getUpperLeft() + drawerSize);
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
        
        if(mFont->measureString(pd).x > getWidth() - 90){
            while( mFont->measureString(pd).x > getWidth() - 90 ){
                pd = pd.substr(0, pd.size()-1);
            }
            pd = pd.substr(0, pd.size()-1);
//            pd.append("…");                     // this might not show up in all fonts
            pd.append("..");                     // this might not show up in all fonts
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
    }else if( this->contains(pos) && mOptions.size() > 1){
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
            mHighlight = (pos.y - mDrawerRect.y1) / getHeight();
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
        gl::drawSolidRect(mBgRect);
        gl::drawSolidRect(mDropDownRect);
        gl::ScopedColor scC(Color::black());
        mDn1->draw();
        mDn2->draw();
        
        if(mOptions.size() > 0){
            gl::ScopedColor scC(Color::black());
            mFont->drawString(mOptions[mIndices[0]], Rectf(mBgRect.getUpperLeft() + vec2(25, 25), mBgRect.getLowerRight() ));
        }
    }else{
        gl::drawSolidRect(mBgRect);
        gl::drawSolidRect(mDropDownRect);
        {
            gl::ScopedMatrices scMat;
            gl::ScopedColor scC(Color::black());
            gl::translate(vec3(9,0,0));
            mDn1->draw();
            gl::translate(vec3(-18,0,0));
            mDn2->draw();
        }
        
        gl::drawSolidRect(Rectf(mBgRect.getLowerLeft()+  vec2(0, 5), mDrawerRect.getLowerRight()));
        
        gl::ScopedColor scC(Color::black());
        for( int i=0; i<mOptions.size(); i++){
            if(mHighlight == i){
                gl::ScopedColor scBg(Color::white());
                gl::drawSolidRect(Rectf(x1, y1 + getHeight() * i, x2, y2 + (getHeight()+1) * i));
            }
            
            mFont->drawString(mOptions[mIndices[i]], (Rectf)*this + vec2(25, 25 + i * getHeight()));
            
            float yp = y2 + getHeight() * i + 1;
            
            if(i != mOptions.size()-1){
                gl::drawLine(vec2(x1+20, yp), vec2(x2-20, yp));
                gl::drawLine(vec2(x1+20, yp+1), vec2(x2-20, yp+1));
            }
        }
        
    }
}



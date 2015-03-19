#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "GFXMain.h"
#include "StateManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

class GoldsprintsFXApp : public AppNative {
  public:
    void prepareSettings( Settings *settings );
	void setup();
	void keyDown( KeyEvent event );
	void update();
	void draw();
    
    void shutdown();
    
    gfx::GFXMain *mGfxMain;
    bool    bDebugState;
};

void GoldsprintsFXApp::prepareSettings(cinder::app::AppBasic::Settings *settings){
    settings->setFrameRate(60.0);
//    settings->setWindowSize(1280, 768);
    settings->setWindowSize(1280, 700);
    settings->setWindowPos((1400-1280)/2, 0 );
    settings->setTitle("SilverSprint");
    
    bDebugState = true;
}

void GoldsprintsFXApp::setup() {
    gl::enableAlphaBlending();
    
    mGfxMain = new gfx::GFXMain();
    mGfxMain->setup();
}

void GoldsprintsFXApp::shutdown(){
    delete mGfxMain;
}

void GoldsprintsFXApp::keyDown( KeyEvent event ){
    if( event.isAccelDown() ){
        if( event.getChar() == 'f' ){
            setFullScreen( !isFullScreen() );
        }
        else if( event.getChar() == '1' ){
            StateManager::getInstance()->changeAppState( APP_STATE::RACE );
        }
        else if( event.getChar() == '2' ){
            StateManager::getInstance()->changeAppState( APP_STATE::ROSTER );
        }
        else if( event.getChar() == '3' ){
            StateManager::getInstance()->changeAppState( APP_STATE::SETTINGS );
        }
    }
}

void GoldsprintsFXApp::update() {
    mGfxMain->update();
}

void GoldsprintsFXApp::draw() {
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::pushMatrices();{
        Rectf originalSize(0,0,1920,1080);
        Rectf scaledFit = originalSize.getCenteredFit(getWindowBounds(), true);
        
        float hScale = getWindowWidth() / originalSize.getWidth();
        float vScale = getWindowHeight() / originalSize.getHeight();
        
        float scaleAmt = ( hScale < vScale ) ? hScale : vScale;
        
        gl::translate( Vec2f(scaledFit.x1, 0) );
        gl::scale(scaleAmt, scaleAmt);
        
        
        mGfxMain->draw( scaledFit );
    }gl::popMatrices();
    
//    gl::drawString( to_string(getAverageFps()), Vec2f(10,10) );
    
    if( bDebugState ){
        gl::drawString( StateManager::getInstance()->getCurrentAppStateString(), Vec2f(10, getWindowHeight() - 40) );
        gl::drawString( StateManager::getInstance()->getCurrentRaceStateString(), Vec2f(10, getWindowHeight() - 20) );
    }
}

CINDER_APP_NATIVE( GoldsprintsFXApp, RendererGl )

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
};

void GoldsprintsFXApp::prepareSettings(cinder::app::AppBasic::Settings *settings){
    settings->setFrameRate(60.0);
//    settings->setWindowSize(1280, 768);
    settings->setWindowSize(1280, 700);
    settings->setWindowPos((1400-1280)/2, 0 );
    settings->setTitle("GoldsprintsFX 2.0");
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
    if( event.getChar() == 'f' ){
        setFullScreen( !isFullScreen() );
    }
    
    else if( event.getChar() == '1' ){
        StateManager::getInstance()->changeState( GFX_STATE::IDLE );
    }
    
    else if( event.getChar() == '2' ){
        StateManager::getInstance()->changeState( GFX_STATE::STAGING );
    }
}

void GoldsprintsFXApp::update() {
    mGfxMain->update();
}

void GoldsprintsFXApp::draw() {
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    mGfxMain->draw();
}

CINDER_APP_NATIVE( GoldsprintsFXApp, RendererGl )

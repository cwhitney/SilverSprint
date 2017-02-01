#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "app/GFXMain.h"
#include "data/GFXGlobal.h"
#include "data/StateManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace gfx;

class SilverSprintApp : public App {
  public:
	void setup() override;
    void resize() override;
	void keyDown( KeyEvent event ) override;
	void update() override;
	void draw() override;
    
    gfx::GFXMainRef mGfxMain;
    bool            bDebugState;
};

void SilverSprintApp::setup()
{
//    log::makeLogger<log::LoggerFile>( getAppPath().string() + "/logs/SilverSprint.log", false );
    
    bDebugState = false;
    
    gl::enableAlphaBlending();
    
    mGfxMain = GFXMainRef( new GFXMain() );
    mGfxMain->setup();
}

void SilverSprintApp::resize()
{
    Rectf originalSize(0,0,1920,1080);
    gfx::GFXGlobal::getInstance()->setScale( originalSize.getCenteredFit(getWindowBounds(), true) );
}

void SilverSprintApp::keyDown( KeyEvent event )
{
    if( event.isAccelDown() || event.isControlDown() ){
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

void SilverSprintApp::update()
{
    mGfxMain->update();
}

void SilverSprintApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );
    
    gl::pushMatrices();{
        Rectf originalSize(0,0,1920,1080);
        Rectf scaledFit = originalSize.getCenteredFit(getWindowBounds(), true);
        
        float hScale = getWindowWidth() / originalSize.getWidth();
        float vScale = getWindowHeight() / originalSize.getHeight();
        
        float scaleAmt = ( hScale < vScale ) ? hScale : vScale;
        
        gl::translate( vec2(scaledFit.x1, 0) );
        gl::scale(scaleAmt, scaleAmt);
        
        mGfxMain->draw( scaledFit );
    }gl::popMatrices();
    
    if( bDebugState ){
        gl::drawString( to_string( getAverageFps() ), vec2(10, getWindowHeight() - 60) );
        gl::drawString( StateManager::getInstance()->getCurrentAppStateString(), vec2(10, getWindowHeight() - 40) );
        gl::drawString( StateManager::getInstance()->getCurrentRaceStateString(), vec2(10, getWindowHeight() - 20) );
    }
}

CINDER_APP( SilverSprintApp, RendererGl(RendererGl::Options().msaa(8)), [&](SilverSprintApp::Settings *settings){
    settings->setFrameRate(60.0);
    settings->setWindowSize(1280, 700);
  //  settings->setWindowPos((1400-1280)/2, 0 );
    settings->setTitle("SilverSprint");
})

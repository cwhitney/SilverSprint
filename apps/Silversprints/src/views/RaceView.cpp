//
//  RaceView.cpp
//  SilverSprints
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "views/RaceView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

RaceView::RaceView() : bVisible(false) {
    
}

void RaceView::setup()
{
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    mLogo = gl::Texture::create( loadImage( loadAsset("img/opensprintsLogo.png") ) );
    
    mDialCenter = vec2(1920.0 * 0.5, 612.0);
    
    for(int i=0; i<4; i++){
        mRaceTexts.push_back( new RaceText( Model::instance().playerColors[i] ) );
    }
    
    mDial = gl::Texture::create( loadImage(loadAsset("img/dial_bg_y.png")) );
    
    mCountDown		= CountDownGfx::create();
    mWinnerModal	= WinnerModal::create();
    
	StateManager::instance().signalOnStateChange.connect(std::bind(&RaceView::onStateChange, this, std::placeholders::_1));

    mStartStop.signalStartRace.connect( std::bind(&RaceView::onStartClicked, this ) );
    mStartStop.signalStopRace.connect( std::bind(&RaceView::onStopClicked, this ) );
    
    mTimerFont = ci::gl::TextureFont::create( ci::Font(loadAsset("fonts/UbuntuMono-B.ttf"), 50.0 / 2.5) );
    
    reloadShader();
    
    setupVbos();
}

void RaceView::reloadShader() {
    try {
        CI_LOG_I("Reloaded shader");
        mProgressShader = gl::GlslProg::create( loadAsset("shaders/RaceProgress.vert"), loadAsset("shaders/RaceProgress.frag") );
    }
    catch( gl::GlslProgCompileExc &exc ) {
        CI_LOG_EXCEPTION("Shader compile error: ", exc);
    }
    catch( ... ) {
        CI_LOG_E("Unable to load shader");
    }
}

void RaceView::setupVbos()
{
    float innerRad = 187;
    float outerRad = 388;
    float radSize = (outerRad - innerRad) / 4.0;
    
    mVboList.push_back( createVbo(innerRad + radSize * 3, innerRad + radSize * 4 ) );
    mVboList.push_back( createVbo(innerRad + radSize * 2, innerRad + radSize * 3 ) );
    mVboList.push_back( createVbo(innerRad + radSize * 1, innerRad + radSize * 2 ) );
    mVboList.push_back( createVbo(innerRad + radSize * 0, innerRad + radSize * 1 ) );
}

ci::gl::VboMeshRef RaceView::createVbo( float innerRad, float outerRad )
{
    ci::gl::VboMeshRef      tmpVbo;
    std::vector<vec3>       vertexBuffer;
    std::vector<uint32_t>   indicesBuffer;
    std::vector<ci::ColorA> colorBuffer;
    std::vector<vec2>       texCoordBuffer;
    
    gl::VertBatchRef vbr = gl::VertBatch::create( GL_TRIANGLE_STRIP );
    
    // BUFFER VERTICES
    int segments = 60;
    
    for( int i=0; i<segments+1; i++ ){
        float degs = 360.0f / (float)segments;
        
        vec3 pti(  cos( toRadians( i * degs - 90 ) ) * innerRad + mDialCenter.x,
                  sin( toRadians( i * degs - 90 ) ) * innerRad + mDialCenter.y,
                  0 );
        
        vec3 pto(  cos( toRadians( i * degs - 90 ) ) * outerRad + mDialCenter.x,
                  sin( toRadians( i * degs - 90 ) ) * outerRad + mDialCenter.y,
                  0 );
        
        float a = ((float)i / (float)segments);

        vbr->color( ColorA(1,1,1,1) );
        vbr->texCoord(a, 0);
        vbr->vertex( pti );
        
        vbr->color( ColorA(1,1,1,1) );
        vbr->texCoord(a, 1);
        vbr->vertex( pto );
    }

    tmpVbo = gl::VboMesh::create( *vbr );
    return tmpVbo;
}

void RaceView::onStartClicked()
{
    if( Model::instance().isHardwareConnected() ){
        StateManager::instance().changeRaceState( RACE_STATE::RACE_STARTING );
    }
}

void RaceView::onStopClicked()
{
    if( Model::instance().isHardwareConnected() ){
        StateManager::instance().changeRaceState( RACE_STATE::RACE_STOPPED );
    }
}

void RaceView::onStateChange(APP_STATE newState)
{
    if( newState == APP_STATE::RACE ){
        bVisible = true;
    }else{
        bVisible = false;
    }
}

void RaceView::update()
{
    mWinnerModal->update();
}

void RaceView::draw()
{
    if( !bVisible ){
        return;
    }
    gl::ScopedColor scC(1,1,1);
    
    if( mBg ){
        gl::draw( mBg );
    }
    
    gl::drawSolidRect( Rectf( 834, 105, 834+260, 105+185 ) );    // big white rect
    gl::drawSolidRect( Rectf( 60, 133, 1870, 135 ) );            // white line
    
    // PLAYER INFO
    for( int i=0; i<Model::instance().getNumRacers(); i++){
        PlayerData *pd = Model::instance().playerData[i];
        mRaceTexts[i]->draw( pd, vec2(0, 390 + 102*i) );
    }
    
    // MAIN TIMER
    gl::ScopedColor scB(0,0,0,1);
    
    if( StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_RUNNING )
    {
        if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_DISTANCE){
            mTimerFont->drawString( sb::utils::millisToTimestamp(Model::instance().elapsedRaceTimeMillis, 2), vec2(867,154) );
        }else{
            double timeRemaining = max(0.0, Model::instance().getRaceLengthMillis() - Model::instance().elapsedRaceTimeMillis);
            mTimerFont->drawString( sb::utils::millisToTimestamp(timeRemaining, 2), vec2(867,154) );
        }
    }else if( StateManager::instance().getCurrentRaceState() == RACE_STATE::RACE_COMPLETE )
    {
        if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_DISTANCE){
            mTimerFont->drawString( sb::utils::millisToTimestamp(Model::instance().elapsedRaceTimeMillis ), vec2(867,154) );
        }else{
            mTimerFont->drawString(sb::utils::millisToTimestamp(0, 2), vec2(867,154) );
        }
    }else {
        mTimerFont->drawString( sb::utils::millisToTimestamp(0, 2), vec2(867,154) );
    }
    
    // DIAL
    gl::ScopedColor scW(1,1,1,1);
    gl::draw( mDial, vec2(mDial->getSize()) * vec2(-0.5) + mDialCenter );
    
    mStartStop.draw();
    
    if( mLogo ){
        gl::color( 1, 1, 1, 1 );
        gl::draw( mLogo, vec2(1920, 1080) - vec2(50,50) - vec2(mLogo->getSize()) );
    }
    
    ci::ColorA tmpCol;
    for( int i=0; i<Model::instance().getNumRacers(); i++){
        PlayerData *pd = Model::instance().playerData[i];
        
        if(mProgressShader){
            gl::ScopedGlslProg scProg( mProgressShader );
            
            float radialPos = pd->getPercent();
            if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_TIME){
                radialPos = pd->getDistanceMeters() / 100.0f;   // 100 meters will be one lap around the track
            }
            float tailLen = lmap(pd->getMph(), 0.0, 30.0, 0.0, 0.30);   // 30mph means a 30% tail
            tailLen = clamp(tailLen, 0.0f, 0.50f);  // no more than 50 though
            
            tmpCol = Model::instance().playerColors[i];
            mProgressShader->uniform( "uBaseColor", tmpCol );
            mProgressShader->uniform( "uLeadingEdgePct", (float)radialPos );
            mProgressShader->uniform( "uTailLen", (float)tailLen );
            
            gl::ScopedColor scCol(Model::instance().playerColors[i]);
            gl::draw( mVboList[i] );
        }
        
        
    }
    
    // GRAPHICS
    mCountDown->draw();
    mWinnerModal->draw();
}





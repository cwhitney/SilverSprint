//
//  RaceView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "RaceView.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

RaceView::RaceView() : bVisible(false) {
    
}

void RaceView::setup()
{
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    mFinishFlag = gl::Texture::create( loadImage( loadAsset("img/finishFlag.png") ) );
    mProgLine = gl::Texture::create( loadImage( loadAsset("img/progLine.png") ) );
    mLogo = gl::Texture::create( loadImage( loadAsset("img/opensprintsLogo.png") ) );
    
    mGlobal = gfx::GFXGlobal::getInstance();
    
    for(int i=0; i<4; i++){
        mRaceTexts.push_back( new RaceText( mGlobal->playerColors[i] ) );
    }
    
    mDial = gl::Texture::create( loadImage(loadAsset("img/dial_bg.png")) );
    
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&RaceView::onStateChange, this, std::placeholders::_1) );
    
    mCountDown = new CountDownGfx();
    
    mStartStop.signalStartRace.connect( std::bind(&RaceView::onStartClicked, this ) );
    mStartStop.signalStopRace.connect( std::bind(&RaceView::onStopClicked, this ) );
    
    mTimerFont = ci::gl::TextureFont::create( ci::Font(loadAsset("fonts/UbuntuMono-B.ttf"), 50) );
    
    reloadShader();
    
    setupVbos();
}

void RaceView::reloadShader() {
    try {
        mProgressShader = gl::GlslProg::create( loadAsset("shaders/RaceProgress.vert"), loadAsset("shaders/RaceProgress.frag") );
    }
    catch( gl::GlslProgCompileExc &exc ) {
        std::cout << "Shader compile error: " << std::endl;
        std::cout << exc.what();
    }
    catch( ... ) {
        std::cout << "Unable to load shader" << std::endl;
    }
}

void RaceView::setupVbos()
{
    float innerRad = 113;
    float outerRad = 333;
    float radSize = (outerRad - innerRad) / 4.0;
    
    mVboList.push_back( createVbo(innerRad + radSize * 3, innerRad + radSize * 4 ) );
    mVboList.push_back( createVbo(innerRad + radSize * 2, innerRad + radSize * 3 ) );
    mVboList.push_back( createVbo(innerRad + radSize * 1, innerRad + radSize * 2 ) );
    mVboList.push_back( createVbo(innerRad + radSize * 0, innerRad + radSize * 1 ) );
}

ci::gl::VboMeshRef RaceView::createVbo( float innerRad, float outerRad )
{
    ci::gl::VboMeshRef      tmpVbo;
    std::vector<vec3>      vertexBuffer;
    std::vector<uint32_t>   indicesBuffer;
    std::vector<ci::ColorA> colorBuffer;
    std::vector<vec2>      texCoordBuffer;
    
    gl::VertBatchRef vbr = gl::VertBatch::create( GL_TRIANGLE_STRIP );
    
    // BUFFER VERTICES
    int segments = 60;
    vec2 center( 967, 612 );
    
    for( int i=0; i<segments+1; i++ ){
        float degs = 360.0f / (float)segments;
        
        vec3 pti(  cos( toRadians( i * degs - 90 ) ) * innerRad + center.x,
                  sin( toRadians( i * degs - 90 ) ) * innerRad + center.y,
                  0 );
        
        vec3 pto(  cos( toRadians( i * degs - 90 ) ) * outerRad + center.x,
                  sin( toRadians( i * degs - 90 ) ) * outerRad + center.y,
                  0 );
        
//        vertexBuffer.push_back( pti );
//        vertexBuffer.push_back( pto );
        
        float a = ((float)i / (float)segments);
        
//        colorBuffer.push_back( ColorA(1,0,1,a) );
//        colorBuffer.push_back( ColorA(1,0,1,a) );
        
//        texCoordBuffer.push_back( vec2(a, 0) );
//        texCoordBuffer.push_back( vec2(a, 1) );
        
        
        vbr->color( ColorA(1,0,1,a) );
        vbr->texCoord(a, 0);
        vbr->vertex( pti );
        
        
        vbr->color( ColorA(1,0,1,a) );
        vbr->texCoord(a, 1);
        vbr->vertex( pto );
        
    }
    
    // BUFFER INDICES
//    for( int i=0; i<vertexBuffer.size(); i++ ){
//        indicesBuffer.push_back( i );
//    }
//    indicesBuffer.push_back( 0 );
//    indicesBuffer.push_back( 1 );
    
    //gl::VboMesh::Layout layout;
    /*
    layout.setStaticPositions();
    layout.setStaticNormals();
    layout.setStaticIndices();
    layout.setStaticColorsRGBA();
    layout.setStaticTexCoords2d();
     */
    
    tmpVbo = gl::VboMesh::create( *vbr );
    
//    tmpVbo = gl::VboMesh::create( vertexBuffer.size(), indicesBuffer.size(), layout, GL_TRIANGLE_STRIP );
//    tmpVbo->bufferIndices( indicesBuffer );
//    tmpVbo->bufferPositions( vertexBuffer );
//    tmpVbo->bufferColorsRGBA( colorBuffer );
//    tmpVbo->bufferTexCoords2d( 0, texCoordBuffer );
    
    return tmpVbo;
}

void RaceView::onStartClicked()
{
    if( mModel->bHardwareConnected ){
        mStateManager->changeRaceState( RACE_STATE::RACE_STARTING );
    }
}

void RaceView::onStopClicked()
{
    if( mModel->bHardwareConnected ){
        mStateManager->changeRaceState( RACE_STATE::RACE_STOPPED );
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
    
}

void RaceView::draw()
{
    if( !bVisible ){
        return;
    }
    
    if( mBg ){
        gl::color(1,1,1);
        gl::draw( mBg );
    }

    gl::drawSolidRect( Rectf( 834, 105, 834+260, 105+185 ) );    // big white rect
    gl::drawSolidRect( Rectf( 60, 133, 1870, 135 ) );            // white line
    
    // PLAYER INFO
    for( int i=0; i<mModel->getNumRacers(); i++){
        PlayerData *pd = Model::getInstance()->playerData[i];
        mRaceTexts[i]->draw( pd, vec2(0, 390 + 102*i) );
    }
    
    // MAIN TIMER
    gl::color(0,0,0,1);
    
    if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_RUNNING ){
//        int elapsedTime = (getElapsedSeconds()*1000.0) - mModel->startTimeMillis;
//        mModel->elapsedRaceTimeMillis = elapsedTime;
        mTimerFont->drawString( mGlobal->toTimestamp(mModel->elapsedRaceTimeMillis ), vec2(867,154) );
    }else if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_COMPLETE ){
        mTimerFont->drawString( mGlobal->toTimestamp(mModel->elapsedRaceTimeMillis ), vec2(867,154) );
    }else {
        mTimerFont->drawString( mGlobal->toTimestamp(0), vec2(867,154) );
    }
    
    // DIAL
    gl::color(1,1,1,1);
    gl::draw( mDial, vec2(mDial->getSize()) * vec2(-0.5) + vec2(967, 580) );
    
    // FLAG
    gl::color(1,1,1,1);
    if( mFinishFlag && mGlobal->currentRaceType == RACE_TYPE::RACE_TYPE_TIME ){
        gl::draw(mFinishFlag, vec2(873, 130));
    }
    
    mStartStop.draw();
    
    if( mLogo ){
        gl::color( 1, 1, 1, 1 );
        gl::draw( mLogo, vec2(1920, 1080) - vec2(50,50) - vec2(mLogo->getSize()) );
    }
    
    ci::ColorA tmpCol;
    for( int i=0; i<mModel->getNumRacers(); i++){
        PlayerData *pd = Model::getInstance()->playerData[i];
        
        if( mProgressShader){
            gl::ScopedGlslProg scProg( mProgressShader );
            
            tmpCol = mGlobal->playerColors[i];
            mProgressShader->uniform( "baseColor", tmpCol );
            mProgressShader->uniform( "leadingEdgePct", (float)pd->getPercent() );
            mProgressShader->uniform( "trailingEdgePct", (float)pd->getPercent() - 0.15f );
            gl::draw( mVboList[i] );
        }
    }
    
    // COUNTDOWN
    mCountDown->draw();
}








//
//  RaceView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#ifdef __linux
    //linux
    #include "../../include/views/RaceView.h"
#else
    // Windows & OSX
    #include "views/RaceView.h"
#endif

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

RaceView::RaceView() : bVisible(false) {

}

void RaceView::setup()
{
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
//    mProgLine = gl::Texture::create( loadImage( loadAsset("img/progLine.png") ) );
    mLogo = gl::Texture::create( loadImage( loadAsset("img/opensprintsLogo.png") ) );

    mGlobal = gfx::GFXGlobal::getInstance();

    mDialCenter = vec2(1920.0 * 0.5, 612.0);

    for(int i=0; i<4; i++){
        mRaceTexts.push_back( new RaceText( mGlobal->playerColors[i] ) );
    }

    mDial = gl::Texture::create( loadImage(loadAsset("img/dial_bg_y.png")) );

    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&RaceView::onStateChange, this, std::placeholders::_1) );

    mCountDown = CountDownGfx::create();
    mWinnerModal = WinnerModal::create();

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

        vbr->color( ColorA(1,0,1,a) );
        vbr->texCoord(a, 0);
        vbr->vertex( pti );

        vbr->color( ColorA(1,0,1,a) );
        vbr->texCoord(a, 1);
        vbr->vertex( pto );
    }

    tmpVbo = gl::VboMesh::create( *vbr );
    return tmpVbo;
}

void RaceView::onStartClicked()
{
    if( mModel->isHardwareConnected() ){
        mStateManager->changeRaceState( RACE_STATE::RACE_STARTING );
    }
}

void RaceView::onStopClicked()
{
    if( mModel->isHardwareConnected() ){
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
    for( int i=0; i<mModel->getNumRacers(); i++){
        PlayerData *pd = Model::getInstance()->playerData[i];
        mRaceTexts[i]->draw( pd, vec2(0, 390 + 102*i) );
    }

    // MAIN TIMER

    gl::ScopedColor scB(0,0,0,1);

    if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_RUNNING ){
        mTimerFont->drawString( mGlobal->toTimestamp(mModel->elapsedRaceTimeMillis ), vec2(867,154) );
    }else if( mStateManager->getCurrentRaceState() == RACE_STATE::RACE_COMPLETE ){
        mTimerFont->drawString( mGlobal->toTimestamp(mModel->elapsedRaceTimeMillis ), vec2(867,154) );
    }else {
        mTimerFont->drawString( mGlobal->toTimestamp(0), vec2(867,154) );
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

    // GRAPHICS
    mCountDown->draw();
    mWinnerModal->draw();
}

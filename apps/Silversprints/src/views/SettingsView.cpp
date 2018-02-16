//
//  SettingsView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/15/14.
//
//

#include "views/SettingsView.h"

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

SettingsView::~SettingsView()
{
    delete mDistanceLabel;
    delete mTimeLabel;
}

SettingsView::SettingsView() : bVisible(false)
{
    mStateManager	= StateManager::getInstance();
    mStateManager->signalOnStateChange.connect( [&](APP_STATE newState, APP_STATE lastState)
                                               {
                                                   onStateChange(newState, lastState);
                                               });
    
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    
    ci::Font fnt(loadAsset("fonts/UbuntuMono-BI.ttf"), 25);
    tFont = gl::TextureFont::create( fnt );
    
    int yPos = 255;
    
    // ROLLER DIAMETER
    mTxtDiameter = makeSetting(Rectf(443,yPos, 443+431, yPos+100), "ROLLER DIAMETER (mm)", "114.3" );
    yPos += 160;
    
    // RACERS
    mTxtNumRacers = makeSetting(Rectf(443,yPos, 443+431 - 50, yPos+100), "NUMBER OF RACERS (1-"+to_string(mMaxRiders)+")", "2" );
    mTxtNumRacers->disable();
    
    int gap = 6;
    mStepperPlus.setup( mTxtNumRacers->getBounds().getUpperRight() + vec2(gap, 0), gap, "+" );
    mStepperPlus.signalOnClick.connect( std::bind(&SettingsView::onStepperPlusClick, this ) );
    mStepperMinus.setup( mTxtNumRacers->getBounds().getUpperRight() + vec2(gap, 100 - 50+gap/2), gap, "-");
    mStepperMinus.signalOnClick.connect( std::bind(&SettingsView::onStepperMinusClick, this ) );
    
    yPos += 160;
    
    // CONNECTION
    mConnectionBox = std::make_shared<CheckBox>(vec2(443, yPos), false);
    mLabels.push_back( TextLabel(mConnectionBox->getBounds().getUpperLeft() - vec2(0, 20), "HARDWARE CONNECTION STATUS") );
    
    // COL 2 ---------------------------------------------------------
    yPos = 255;
    mDistanceCheck = std::make_shared<CheckBox>(vec2(1043, yPos));
    mDistanceCheck->setChecked(true);
    mTimeTrialBox = std::make_shared<CheckBox>(vec2(1343-30, yPos));
    
    yPos += 160;
    
    // RACE DISTANCE
    mTxtDistance = makeTextField(Rectf(1043,yPos, 1043+431, yPos+100), "100" );
    mTxtTime = makeTextField(Rectf(1043,yPos, 1043+431, yPos+100), "60" );
    
    mDistanceLabel = new TextLabel(vec2(1043,yPos) - vec2(0,20), "RACE LENGTH (meters)");
    mTimeLabel = new TextLabel(vec2(1043,yPos) - vec2(0,20), "RACE TIME (seconds)");
    mTxtTime->visible = false;
    
    mDistanceCheck->signalOnClick.connect([&](){
        mTimeTrialBox->setChecked(false);
        mTxtDistance->visible = true;
        mTxtTime->visible = false;
    });
    mTimeTrialBox->signalOnClick.connect([&](){
        mDistanceCheck->setChecked(false);
        mTxtDistance->visible = false;
        mTxtTime->visible = true;
    });
    
    // KPH
    yPos += 160;
    //yPos = 255;
    float xPos = 1043;
    mLabels.push_back( TextLabel(vec2(xPos, yPos-20), "UNITS") );
    auto toggleFont =gl::TextureFont::create(ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 70));
    mMphKphToggle = std::make_shared<ToggleBtn>("MPH", "KPH", toggleFont, vec2(xPos, yPos));
    mMphKphToggle->setColors( Model::instance().playerColors[0], Color::black() );
    mMphKphToggle->setSelected(ToggleBtn::TOGGLE_SIDE::RIGHT);
    
    // Race Logging
    yPos += 160;
    mLabels.push_back( TextLabel(vec2(xPos, yPos-20), "LOG RACES TO FILE") );
    mRaceLoggingBox = std::make_shared<CheckBox>(vec2(1043, yPos));
    mRaceLoggingBox->setChecked(false);
    mRaceLoggingBox->signalOnClick.connect([&](){
        Model::instance().setRaceLogging(mRaceLoggingBox->isChecked());
    });
}

void SettingsView::onStepperPlusClick()
{
    int numRacers = fromString<int>( mTxtNumRacers->getText() );
    mTxtNumRacers->setText( toString( min( ++numRacers, mMaxRiders) ) );
}

void SettingsView::onStepperMinusClick()
{
    int numRacers = fromString<int>( mTxtNumRacers->getText() );
    mTxtNumRacers->setText( toString( max( --numRacers, 1) ) );
}

CiTextField* SettingsView::makeSetting(Rectf rect, std::string label, std::string txt)
{
    mLabels.push_back( TextLabel(vec2(rect.getUpperLeft() - vec2(0,20)), label) );
    return makeTextField(rect, txt);
}

CiTextField* SettingsView::makeTextField(ci::Rectf rect, std::string txt)
{
    CiTextField *tf = new CiTextField(txt, rect, ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 70) );
    tf->mColorStroke = Model::instance().playerColors[0];
    tf->mColorFill = Model::instance().playerColors[0];
    tf->mColorText = Color::black();
    tf->padding = vec2(20,0);
    
    tf->bUseScissorTest = false;
    
    return tf;
}

void SettingsView::onStateChange(APP_STATE newState, APP_STATE lastState)
{
    if( newState == APP_STATE::SETTINGS ){
        bVisible = true;
        mTxtDiameter->visible = true;
        mTxtNumRacers->visible = true;
        
        mMphKphToggle->setSelected(Model::instance().getUsesKph() ? ToggleBtn::TOGGLE_SIDE::RIGHT : ToggleBtn::TOGGLE_SIDE::LEFT);
        mTxtDiameter->setText(toString<float>(Model::instance().getRollerDiameterMm()));
        mTxtNumRacers->setText(toString<int>(Model::instance().getNumRacers()));
        mTxtDistance->setText(toString<float>(Model::instance().getRaceLengthMeters()));
        mTxtTime->setText(toString<float>(Model::instance().getRaceTimeSeconds()));
        
        if(Model::instance().getCurrentRaceType() == Model::RACE_TYPE_DISTANCE){
            mDistanceCheck->setChecked(true);
            mTimeTrialBox->setChecked(false);
            mTxtDistance->visible = true;
        }else{
            mDistanceCheck->setChecked(false);
            mTimeTrialBox->setChecked(true);
            mTxtTime->visible = true;
        }
        mRaceLoggingBox->setChecked( Model::instance().getRaceLogging());

    }else if(lastState == APP_STATE::SETTINGS){
		CI_LOG_I("Settings ::  Updating model");
        Model::instance().setRollerDiameterMm( fromString<float>(mTxtDiameter->getText()));
        Model::instance().setNumRacers( fromString<int>(mTxtNumRacers->getText()) );
        Model::instance().setRaceLengthMeters( fromString<float>(mTxtDistance->getText()) );

        Model::instance().setRaceTimeSeconds( fromString<float>(mTxtTime->getText()) );
        
        Model::instance().setCurrentRaceType( (mDistanceCheck->isChecked()) ? Model::RACE_TYPE_DISTANCE : Model::RACE_TYPE::RACE_TYPE_TIME);
        
        Model::instance().setUseKph(mMphKphToggle->getSelected() == ToggleBtn::TOGGLE_SIDE::RIGHT);
        bVisible = false;
        
        mTxtDiameter->visible = false;
        mTxtDistance->visible = false;
        mTxtNumRacers->visible = false;
    }
}

void SettingsView::update()
{
    
}

void SettingsView::draw()
{
    gl::color(1,1,1,1);
    if( bVisible ){
        if( mBg ){
            gl::draw(mBg);
        }
        gl::color(1,1,1,1);
        mTxtDiameter->draw();
        mTxtNumRacers->draw();
        
        gl::ScopedColor scGr( Color::gray(0.55) );
        for( int i=0; i<mLabels.size(); i++){
            tFont->drawString(mLabels[i].txt, mLabels[i].pos);
        }
        
        mStepperPlus.draw();
        mStepperMinus.draw();
        
        if( Model::instance().isHardwareConnected() ){
            mConnectionBox->setChecked(true);
            
            gl::ScopedColor scGr( Color::gray(0.55) );
            tFont->drawString("FIRMWARE VERSION", mConnectionBox->getBounds().getLowerRight() + vec2(10, -30));
            tFont->drawString(Model::instance().mFirmwareVersion, mConnectionBox->getBounds().getLowerRight() + vec2(10, -5));

        }else{
            mConnectionBox->setChecked(false);
        }
        mConnectionBox->draw();
        
        // COLUMN 2 ----------------
        mDistanceCheck->draw();
        mTimeTrialBox->draw();
        
        mTxtDistance->draw();
        mTxtTime->draw();
        
        {
            gl::ScopedColor scGr( Color::gray(0.55) );
            if(mDistanceCheck->isChecked()){
                tFont->drawString(mDistanceLabel->txt, mDistanceLabel->pos);
            }else{
                tFont->drawString(mTimeLabel->txt, mTimeLabel->pos);
            }
            tFont->drawString("DISTANCE\nRACE", mDistanceCheck->getBounds().getUpperRight() + vec2(10, 15));
            tFont->drawString("TIME\nRACE", mTimeTrialBox->getBounds().getUpperRight() + vec2(10, 15));
        }
        
        // KPH
        {
            mMphKphToggle->draw();
        }
        {
            gl::ScopedColor scGr( Color::gray(0.55) );
            tFont->drawString("SILVERSPRINT VERSION: " + std::string(SILVERSPRINT_VERSION_STR), vec2(60, 1080- 20));
        }
        
        // RACE LOGGING
        {
            mRaceLoggingBox->draw();
        }
    }
}

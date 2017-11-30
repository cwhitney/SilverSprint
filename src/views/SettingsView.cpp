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
    mGlobal			= gfx::GFXGlobal::getInstance();
    mStateManager	= StateManager::getInstance();
    mModel			= Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&SettingsView::onStateChange, this, std::placeholders::_1) );
    
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    
    ci::Font fnt(loadAsset("fonts/UbuntuMono-BI.ttf"), 25);
    tFont = gl::TextureFont::create( fnt );
    
    int yPos = 255;
    
    // ROLLER DIAMETER
    mTxtDiameter = makeSetting(Rectf(443,yPos, 443+431, yPos+100), "ROLLER DIAMETER (mm)", "114.3" );
    yPos += 160;
    
    // RACERS
    mTxtNumRacers = makeSetting(Rectf(443,yPos, 443+431 - 50, yPos+100), "NUMBER OF RACERS (1-3)", "2" );
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
    mMphKphToggle->setColors( mGlobal->playerColors[0], Color::black() );
    mMphKphToggle->setSelected(ToggleBtn::TOGGLE_SIDE::RIGHT);
    
    ci::app::WindowRef win = getWindow();
    win->getSignalMouseUp().connect(std::bind(&SettingsView::onMouseUp, this, std::placeholders::_1));
}

void SettingsView::onStepperPlusClick()
{
    int numRacers = fromString<int>( mTxtNumRacers->getText() );
    mTxtNumRacers->setText( toString( min( ++numRacers, 3) ) );
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
    tf->mColorStroke = mGlobal->playerColors[0];
    tf->mColorFill = mGlobal->playerColors[0];
    tf->mColorText = Color::black();
    tf->padding = vec2(20,0);
    
    tf->bUseScissorTest = false;
    
    return tf;
}

void SettingsView::onMouseUp(ci::app::MouseEvent event){
    
}

void SettingsView::onStateChange(APP_STATE newState)
{
    if( newState == APP_STATE::SETTINGS ){
        bVisible = true;
        mTxtDiameter->visible = true;
        mTxtNumRacers->visible = true;
        
        mMphKphToggle->setSelected(mModel->getUsesKph() ? ToggleBtn::TOGGLE_SIDE::RIGHT : ToggleBtn::TOGGLE_SIDE::LEFT);
        mTxtDiameter->setText(toString<float>(mModel->getRollerDiameterMm()));
        mTxtNumRacers->setText(toString<int>(mModel->getNumRacers()));
        mTxtDistance->setText(toString<float>(mModel->getRaceLengthMeters()));
        mTxtTime->setText(toString<float>(mModel->getRaceTimeSeconds()));
        
        if(mGlobal->currentRaceType == RACE_TYPE_DISTANCE){
            mDistanceCheck->setChecked(true);
            mTimeTrialBox->setChecked(false);
            mTxtDistance->visible = true;
        }else{
            mDistanceCheck->setChecked(false);
            mTimeTrialBox->setChecked(true);
            mTxtTime->visible = true;
        }

    }else{
		CI_LOG_I("Settings ::  Updating model");
        mModel->setRollerDiameterMm( fromString<float>(mTxtDiameter->getText()));
        mModel->setNumRacers( fromString<int>(mTxtNumRacers->getText()) );
        mModel->setRaceLengthMeters( fromString<float>(mTxtDistance->getText()) );

        mModel->setRaceTimeSeconds( fromString<float>(mTxtTime->getText()) );
        
        GFXGlobal::getInstance()->currentRaceType = (mDistanceCheck->isChecked()) ? RACE_TYPE_DISTANCE : RACE_TYPE::RACE_TYPE_TIME;
        
        mModel->setUseKph(mMphKphToggle->getSelected() == ToggleBtn::TOGGLE_SIDE::RIGHT);
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
        
        if( mModel->isHardwareConnected() ){
            mConnectionBox->setChecked(true);
            
            gl::ScopedColor scGr( Color::gray(0.55) );
            tFont->drawString("FIRMWARE VERSION", mConnectionBox->getBounds().getLowerRight() + vec2(10, -30));
            tFont->drawString(mModel->mFirmwareVersion, mConnectionBox->getBounds().getLowerRight() + vec2(10, -5));

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
    }
}

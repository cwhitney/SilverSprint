//
//  SettingsView.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 10/15/14.
//
//

#include "SettingsView.h"

using namespace gfx;
using namespace ci;
using namespace ci::app;
using namespace std;

SettingsView::SettingsView() : bVisible(false)
{
    mGlobal = gfx::GFXGlobal::getInstance();
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&SettingsView::onStateChange, this, std::placeholders::_1) );
    
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    
    ci::Font fnt(loadAsset("fonts/UbuntuMono-BI.ttf"), 25);
    tFont = gl::TextureFont::create( fnt );

    int yPos = 255;
    
    // ROLLER DIAMETER
    mTxtDiameter = makeSetting(Rectf(443,yPos, 443+431, yPos+100), "ROLLER DIAMETER (mm)", "82.55" );
    yPos += 160;
    
    // RACE DISTANCE
    mTxtDistance = makeSetting(Rectf(443,yPos, 443+431, yPos+100), "RACE LENGTH (meters)", "100" );
    yPos += 160;
    
    // RACERS
    mTxtNumRacers = makeSetting(Rectf(443,yPos, 443+431 - 50, yPos+100), "NUMBER OF RACERS (1-4)", "2" );
    mTxtNumRacers->disable();
    
    int gap = 6;
    mStepperPlus.setup( mTxtNumRacers->getBounds().getUpperRight() + vec2(gap, 0), gap, "+" );
    mStepperPlus.signalOnClick.connect( std::bind(&SettingsView::onStepperPlusClick, this ) );
    mStepperMinus.setup( mTxtNumRacers->getBounds().getUpperRight() + vec2(gap, 100 - 50+gap/2), gap, "-");
    mStepperMinus.signalOnClick.connect( std::bind(&SettingsView::onStepperMinusClick, this ) );
    
    yPos += 160;
    
    // CONNECTION
    mConnectionRect = Rectf(443, yPos, 443+100, yPos+100);
    mLabels.push_back( TextLabel(mConnectionRect.getUpperLeft() - vec2(0, 20), "HARDWARE CONNECTION STATUS") );
    
    mXLine1 = ThickLine::create( mConnectionRect.getUpperLeft()  + vec2(20,20),  mConnectionRect.getLowerRight() - vec2(20,20), 5);
    mXLine2 = ThickLine::create( mConnectionRect.getUpperRight() + vec2(-20,20), mConnectionRect.getLowerLeft() + vec2(20,-20), 5);
    
    vec2 p1(20,60), p2(40,80), p3(80,20);
    vec2 t = mConnectionRect.getUpperLeft();;
    mCheckLine1 = ThickLine::create( p1+t, p2+t + vec2(1.5,1.5), 5 );
    mCheckLine2 = ThickLine::create( p2+t, p3+t, 5 );
    
    
    ci::app::WindowRef win = getWindow();
    win->getSignalMouseUp().connect(std::bind(&SettingsView::onMouseUp, this, std::placeholders::_1));
}

void SettingsView::onStepperPlusClick()
{
    int numRacers = fromString<int>( mTxtNumRacers->getText() );
    mTxtNumRacers->setText( toString( min( ++numRacers, 4) ) );
}

void SettingsView::onStepperMinusClick()
{
    int numRacers = fromString<int>( mTxtNumRacers->getText() );
    mTxtNumRacers->setText( toString( max( --numRacers, 1) ) );
}

CiTextField* SettingsView::makeSetting(Rectf rect, std::string label, std::string txt)
{
    CiTextField *tf = new CiTextField(txt, rect, ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 70) );
    tf->mColorStroke = mGlobal->playerColors[0];
    tf->mColorFill = mGlobal->playerColors[0];
    tf->mColorText = Color::black();
    tf->padding = vec2(20,0);
    
    tf->bUseScissorTest = false;
    
    mLabels.push_back( TextLabel(vec2(rect.getUpperLeft() - vec2(0,20)), label) );
    
    return tf;
}

void SettingsView::onMouseUp(ci::app::MouseEvent event){
    
}

void SettingsView::onStateChange(APP_STATE newState)
{
    if( newState == APP_STATE::SETTINGS ){
        bVisible = true;
        mTxtDiameter->visible = true;
        mTxtDistance->visible = true;
        mTxtNumRacers->visible = true;
    }else{
        mModel->setRollerDiameterMm( fromString<float>(mTxtDiameter->getText()));
        mModel->setNumRacers( fromString<int>(mTxtNumRacers->getText()) );
        mModel->setRaceLengthMeters( fromString<int>(mTxtDistance->getText()) );
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
        mTxtDistance->draw();
        mTxtNumRacers->draw();
        
        gl::color( Color::gray(0.55) );
        for( int i=0; i<mLabels.size(); i++){
            tFont->drawString(mLabels[i].txt, mLabels[i].pos);
        }
        
        mStepperPlus.draw();
        mStepperMinus.draw();
        
        gl::ScopedColor scPc( mGlobal->playerColors[0] );
        gl::drawSolidRect( mConnectionRect );
        
        gl::ScopedColor scCol(Color::gray(0.1));

        if( mModel->bHardwareConnected ){
            mCheckLine1->draw();
            mCheckLine2->draw();
        }else{
            mXLine1->draw();
            mXLine2->draw();
        }
    }
}
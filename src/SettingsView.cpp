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

SettingsView::SettingsView() : bVisible(false) {
    mGlobal = gfx::GFXGlobal::getInstance();
    mStateManager = StateManager::getInstance();
    mModel = Model::getInstance();
    mStateManager->signalOnStateChange.connect( std::bind(&SettingsView::onStateChange, this, std::placeholders::_1) );
    
    mBg = gl::Texture::create( loadImage( loadAsset("img/bgGrad.png") ) );
    
    ci::Font fnt(loadAsset("fonts/UbuntuMono-BI.ttf"), 25);
    tFont = gl::TextureFont::create( fnt );

    mTextFieldList.push_back( makeSetting(Rectf(443,255, 443+431, 255+147), "ROLLER DIAMETER (mm)", "82.55") );
}

CiTextField* SettingsView::makeSetting(Rectf rect, std::string label, std::string txt){
    CiTextField *tf = new CiTextField(txt, rect, ci::Font(loadAsset("fonts/UbuntuMono-R.ttf"), 70) );
    tf->mColorStroke = mGlobal->playerColors[0];
    tf->mColorFill = mGlobal->playerColors[0];
    tf->mColorText = Color::black();
    tf->padding = Vec2f(30,30);
    
    tf->bUseScissorTest = false;
    
    
    mLabels.push_back( TextLabel(Vec2f(rect.getUpperLeft() - Vec2f(0,20)), label) );
    
    return tf;
}

void SettingsView::onStateChange(GFX_STATE newState) {
    if( newState == GFX_STATE::SETTINGS ){
        bVisible = true;
    }else{
        mModel->rollerDiameterMm = mTextFieldList[0]->getText();
        bVisible = false;
    }
}

void SettingsView::update(){
    
}

void SettingsView::draw(){
    gl::color(1,1,1,1);
    if( bVisible ){
        if( mBg ){
            gl::draw(mBg);
        }
        gl::color(1,1,1,1);
        for( auto it = mTextFieldList.begin(); it!=mTextFieldList.end(); ++it){
            (*it)->draw();
        }
        
        gl::color( Color::gray(0.55) );
        for( int i=0; i<mLabels.size(); i++){
            tFont->drawString(mLabels[i].txt, mLabels[i].pos);
        }
    }
}
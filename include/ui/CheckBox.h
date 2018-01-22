#pragma once

#include "cinder/app/App.h"
#include "data/GFXGlobal.h"
#include "data/Model.h"
#include "ui/ThickLine.h"
#include "ui/BaseButton.h"

using CheckBoxRef = std::shared_ptr<class CheckBox>;

class CheckBox : public BaseButton {
  public:
    CheckBox(ci::vec2 position, bool interactive = true);
    
    void setChecked(bool checked=true){ bChecked = checked; }
    void draw();
    
    bool isChecked();
    
    const ci::vec2 getPosition(){ return mBounds.getUpperLeft(); };
    const ci::Rectf getBounds(){ return mBounds; };
    
    ci::signals::Signal<void(void)>	signalOnClick;
    
  protected:
    virtual void onMouseOver();
    virtual void onMouseOut();
    virtual void onClick();

    //ci::Rectf   mBounds;
    bool        bInteractive = true;
    bool        bChecked = false;
    
    gfx::Model           *mModel;
    gfx::GFXGlobal       *mGlobal;
    
    ThickLineRef    mXLine1, mXLine2, mCheckLine1, mCheckLine2;
};

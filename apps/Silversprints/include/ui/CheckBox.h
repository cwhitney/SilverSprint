#pragma once

#include "cinder/app/App.h"
#include "cinder/Rect.h"
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
    
    const ci::vec2 getPosition(){ return BaseButton::getUpperLeft(); }
    const ci::Rectf getBounds(){ return (ci::Rectf)*this; }
    
    ci::signals::Signal<void(void)>	signalOnClick;
    
  protected:
    virtual void onMouseOver();
    virtual void onMouseOut();
    virtual void onClick();

    bool        bInteractive = true;
    bool        bChecked = false;
    
    ThickLineRef    mXLine1, mXLine2, mCheckLine1, mCheckLine2;
};

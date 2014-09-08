//
//  GFXGlobal.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#include "GFXGlobal.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

GFXGlobal* GFXGlobal::mInstance = NULL;
 
GFXGlobal * GFXGlobal::getInstance(){
    if (!mInstance){
        mInstance = new GFXGlobal();
    }
    return mInstance;
}
 
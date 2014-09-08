//
//  Model.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#include "Model.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gfx;

Model* Model::mInstance = NULL;

Model * Model::getInstance(){
    if (!mInstance){
        mInstance = new Model();
    }
    return mInstance;
}

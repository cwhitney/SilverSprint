//
//  Model.h
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/28/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

#include "PlayerData.h"

class Model {
  public:
	static Model* getInstance();
    
  private:
	// PRIVATE CONSTRUCTOR + COPY
	Model(){};
	Model(Model const&){};
	static Model		*mInstance;
};
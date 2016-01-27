#pragma once

#include "cinder/app/App.h"

class SSManager {
public:
    static SSManager* getInstance();
    
private:
    // PRIVATE CONSTRUCTOR + COPY
    SSManager(){};
    SSManager(SSManager const&){};
    static SSManager		*mInstance;
};

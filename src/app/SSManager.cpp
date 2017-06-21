//
//  SSManager.cpp
//  GoldsprintsFX
//
//  Created by Charlie Whitney on 8/27/14.
//
//

#ifdef __linux
    //linux
    #include "../../include/app/SSManager.h"
#else
    // Windows & OSX
    #include "app/SSManager.h"
#endif

SSManager* SSManager::mInstance = NULL;

SSManager * SSManager::getInstance(){
    if (!mInstance){
        mInstance = new SSManager();
    }
    return mInstance;
}

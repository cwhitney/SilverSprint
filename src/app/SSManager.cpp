#include "SSManager.h"

SSManager* SSManager::mInstance = NULL;

SSManager * SSManager::getInstance(){
    if (!mInstance){
        mInstance = new SSManager();
    }
    return mInstance;
}
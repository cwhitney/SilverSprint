#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/GlslProg.h"

using ThickLineRef = std::shared_ptr<class ThickLine>;

class ThickLine {
  public:
    static ThickLineRef create(ci::vec2 start, ci::vec2 end, float thickness){
        return ThickLineRef( new ThickLine(start, end, thickness) );
    }
    
    void draw();
    
  private:
    ThickLine(ci::vec2 start, ci::vec2 end, float thickness);
    
    ci::gl::BatchRef    mBatch;
};
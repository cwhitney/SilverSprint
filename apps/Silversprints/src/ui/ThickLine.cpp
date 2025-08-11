#include "ui/ThickLine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ThickLine::ThickLine(ci::vec2 start, ci::vec2 end, float thickness)
{
    gl::VertBatchRef vbr = gl::VertBatch::create( GL_TRIANGLE_STRIP );
    
    vec2 dir = normalize(end - start);
    vec2 crs( dir.y, -dir.x);
    vec2 girth = crs * vec2(thickness * 0.5f);
    
    // -------
    vbr->texCoord(0, 0);
    vbr->vertex( vec3(start - girth, 0.0) );
    
    vbr->texCoord(0, 1);
    vbr->vertex( vec3(start + girth, 0.0) );
    
    vbr->texCoord(1, 0);
    vbr->vertex( vec3(end - girth, 0.0) );
    
    vbr->texCoord(1, 1);
    vbr->vertex( vec3(end + girth, 0.0) );
    
    ci::gl::VboMeshRef vbo = gl::VboMesh::create( *vbr );
    mBatch = gl::Batch::create( vbo, gl::getStockShader(gl::ShaderDef().color()) );
}

void ThickLine::draw()
{
    mBatch->draw();
}
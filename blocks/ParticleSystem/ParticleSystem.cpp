#include "ParticleSystem.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sharkbox;

ParticleSystem::ParticleSystem()
{
    setupParticles();
}

void ParticleSystem::resetSystem()
{
    
    auto curPar = mParticleList.begin();
    auto curParData = mParticleDataList.begin();
    
    for(; curPar != mParticleList.end(); ){
        curPar->age = 0;
        curPar->pos = vec3(0);
        curPar->vel = vec3(0);
        curPar->rot = vec3(0);
        curPar->size = vec3(0);
        curPar->bAlive = false;
        
        curParData->transform = makeMat(&*curPar);

        ++curPar;
        ++curParData;
    }
    
    // copy to our vbo
    ParticleData *data = (ParticleData*)mInstanceDataVbo->mapReplace();
    memcpy( data, mParticleDataList.data(), mParticleDataList.size() * sizeof(ParticleData) );
    mInstanceDataVbo->unmap();
    
}

void ParticleSystem::setupParticles()
{
    CI_ASSERT(mParticleList.size() == mParticleDataList.size() );
    
    while( mParticleList.size() > maxParticles){
        mParticleList.pop_back();
        mParticleDataList.pop_back();
    }
    while( mParticleList.size() < maxParticles ){
        Particle p;
        mParticleList.push_back(p);
        
        ParticleData pd;
        pd.transform = makeMat(&p);
        mParticleDataList.push_back(pd);
    }
    
    createBatch();
}

void ParticleSystem::createBatch()
{
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, mParticleDataList.size() * sizeof(ParticleData), mParticleDataList.data(), GL_DYNAMIC_DRAW );
    
    // Setup attribute matching
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 16, sizeof( ParticleData ), offsetof(ParticleData, transform), 1 );
    
    // Add to our mesh
//    gl::VboMeshRef mPlaneVbo = gl::VboMesh::create(ci::geom::Sphere().radius(1));
    gl::VboMeshRef mPlaneVbo = gl::VboMesh::create(ci::geom::Plane().size(vec2(1)).axes(vec3(1,0,0), vec3(0,-1,0)));
    mPlaneVbo->appendVbo( instanceDataLayout, mInstanceDataVbo );
    
    // BUILD BATCH
    gl::Batch::AttributeMapping mapping;
    mapping[geom::Attrib::CUSTOM_0] = "transformMat";
    
    // load instanced shader
//    mInstancedShader = gl::GlslProg::create(loadAsset("ParticleInstanced.vert"), loadAsset("ParticleInstanced.frag"));
    mInstancedShader = gl::GlslProg::create(vertShader, fragShader);
    
    mParticleBatch = gl::Batch::create( mPlaneVbo, mInstancedShader, mapping );
}

void ParticleSystem::emitParticle(Particle *p)
{
    p->age = 0.0f;
    p->lifespan = particleLifespan + randFloat(-particleLifespan * mLifespanRandom, particleLifespan * mLifespanRandom);
    p->pos = emitterPos;
    
    if(emitterSize != vec3(0)){
        vec3 hE = emitterSize * vec3(0.5);
        p->pos += vec3( randFloat(-hE.x, hE.x),
                       randFloat(-hE.y, hE.y),
                       randFloat(-hE.z, hE.z) );
    }
    
    float rVel = randFloat(-emitVel * emitVelRand, emitVel * emitVelRand);
    p->vel = normalize(emitDir) * vec3(emitVel + rVel);
    p->size = mParticleSize;
    p->rot = particleRotAngle;
    p->rotAngle = 0;
    p->bAlive = true;
}

ci::mat4 ParticleSystem::makeMat(Particle *particle)
{
    mat4 m;
    m = glm::translate(m, particle->pos);
    m = glm::rotate(m, particle->rotAngle, particle->rot);
    m = glm::scale(m, vec3(particle->size) );

    return m;
}

void ParticleSystem::update(double dt)
{
    // first time only
    if(mCurTime < 0){
        mCurTime = 0;
        mLastSpawn = 0;
    }
    mCurTime += dt;
    
    // SHOULD WE SPAWN MORE?
    int numToSpawn = 0;
    float spawnEvery = 1.0f / emitRatePerSec;
    while( mLastSpawn + spawnEvery < mCurTime ){
        mLastSpawn += spawnEvery;
        ++numToSpawn;
    }
    
    // UPDATE EXISTING, SPAWN NEW, AND REMOVE DEAD ---------
    auto curPar = mParticleList.begin();
    auto curParData = mParticleDataList.begin();
    
    for(; curPar != mParticleList.end(); ){
        
        if( curPar->bAlive){                    // UPDATE EXISTING
            curPar->age += dt;
            curPar->pos += curPar->vel;
            curPar->rotAngle += particleRotSpeed;
            
            if(curPar->age > curPar->lifespan){ // DEAD
                curPar->bAlive = false;
                curPar->size = vec3(0);
            }
            
            curParData->transform = makeMat(&*curPar);
        }else {
            if(numToSpawn > 0){                 // SPAWN NEW
                numToSpawn--;
                emitParticle(&*curPar);
            }
        }
        
        ++curPar;
        ++curParData;
    }
    
    // copy to our vbo
    ParticleData *data = (ParticleData*)mInstanceDataVbo->mapReplace();
    memcpy( data, mParticleDataList.data(), mParticleDataList.size() * sizeof(ParticleData) );
    mInstanceDataVbo->unmap();
}

void ParticleSystem::draw()
{
    mParticleBatch->drawInstanced(maxParticles);
}

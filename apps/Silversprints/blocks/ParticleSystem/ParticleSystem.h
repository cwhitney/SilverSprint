#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

namespace sharkbox {

    struct Particle {
        ci::vec3    pos = ci::vec3(9999);
        ci::vec3    vel;
        ci::vec3    rot;
        float       rotAngle;
        
        float       age = 0.0f;
        float       lifespan = 3.0;
        bool        bAlive = false;
        ci::vec3    size = ci::vec3(0);
    };
    
    struct ParticleData {
        glm::mat4   transform;
    };

    using ParticleSystemRef = std::shared_ptr<class ParticleSystem>;
    class ParticleSystem {
      public:
        ParticleSystem();
        
        void update(double dt);
        void draw();
        
        void resetSystem();
        
        // emitting
        ci::vec3            emitterPos;
        ci::vec3            emitterSize = ci::vec3(0);
        ci::vec3            emitDir = ci::vec3(0,1,0);
        int                 emitRatePerSec = 10;
        float               emitVel = 1;
        float               emitVelRand = 0.2;
        
        // particles
        int                 maxParticles = 500;
        float               particleLifespan = 3.0; //secs
        float               mLifespanRandom = 0.2;
        ci::vec3            mParticleSize = ci::vec3(1);
        float               mSizeRandom = 0.0;
        ci::gl::TextureRef  particleTex = nullptr;
        ci::vec3            particleRotAngle;
        float               particleRotRand;
        float               particleRotSpeed;
        
      protected:
        void setupParticles();
        void createBatch();
        void emitParticle(Particle *p);
        ci::mat4 makeMat( Particle *particle );
        
        std::vector<Particle>       mParticleList;
        std::vector<ParticleData>   mParticleDataList;
        
        ci::gl::VboRef          mInstanceDataVbo;
        ci::gl::GlslProgRef     mInstancedShader;
        ci::gl::BatchRef        mParticleBatch;
        
        double mCurTime         = -1;
        double mLastSpawn       = 0;
        
        std::string vertShader ="#version 150 core\n"
        "\n"
        "uniform mat4	ciModelViewProjection;\n"
        "uniform mat3	ciNormalMatrix;\n"
        "uniform mat4  	ciModelView;\n"
        "\n"
        "in vec4     ciColor;\n"
        "in vec3		ciNormal;\n"
        "in vec4     ciPosition;\n"
        "in vec2		ciTexCoord0;\n"
        "\n"
        "// CUSTOM\n"
        "in mat4     	transformMat;\n"
        "// in vec3			vPos;\n"
        "\n"
        "out lowp vec4   vColor;\n"
        "out highp vec2  vTexCoord;\n"
        "out highp vec3  vNormal;\n"
        "out vec4 		vPosition;\n"
        "\n"
        "float lmap(float val, float inMin, float inMax, float outMin, float outMax)\n"
        "{\n"
        "    return outMin + (outMax - outMin) * ((val - inMin) / (inMax - inMin));\n"
        "}\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    vColor    	= ciColor;\n"
        "    vTexCoord	= ciTexCoord0;\n"
        "    vNormal    	= ciNormalMatrix * ( (transformMat * vec4(ciNormal, 0.0))).xyz;\n"
        "	vPosition 	= ciModelView * transformMat * ciPosition;\n"
        ""
        "    gl_Position = ciModelViewProjection * transformMat * ciPosition;\n"
        "}";
        
        std::string fragShader = "#version 150 core\n"
        "\n"
        "uniform sampler2D uTex0;\n"
        "\n"
        "in vec4	        vColor;\n"
        "in vec2	        vTexCoord;\n"
        "in vec3         vNormal;\n"
        "in vec4         vPosition;\n"
        "\n"
        "out vec4 	oColor;\n"
        "\n"
        "float lmap(float val, float inMin, float inMax, float outMin, float outMax)\n"
        "{\n"
        "    return outMin + (outMax - outMin) * ((val - inMin) / (inMax - inMin));\n"
        "}\n"
        "\n"
        "void main( void )\n"
        "{\n"
        "    vec3 lightPos = vec3(0,1,1);\n"
        "\n"
        "	vec3 Normal        = normalize( vNormal );\n"
        "    vec3 LightVec       = normalize( lightPos - vPosition.xyz );\n"
        "    float NdotL         = max( dot( vNormal, LightVec ), 0.0 );\n"
        "    \n"
        "    vec3 Diffuse        = vec3( NdotL );\n"
        "    vec3 Ambient        = vec3( 0.5 );\n"
        "    \n"
        "    oColor = vColor * vec4( vec3(Diffuse + Ambient), 1.0);\n"
        "}";
    };
}

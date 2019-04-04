//
//  Starfall.cpp
//  Starfall
//
//  Created by lingtonke on 2019/3/19.
//  Copyright © 2019 Starfall. All rights reserved.
//

#include "SFParticleSystem.h"
#include "ObjectiveGL.h"
#include "SFParticleNode.h"
#include "SFParticleShader.h"
#include <sstream>

#define TEST

using namespace Starfall;

void SFParticleSystem::setup(SFParticleConfig config)
{
    this->config = config;
    auto context = GLContext::current();
    
    transformMatrix =
    {1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1,
    };
    
    vbo = GLBuffer::create();
    vbo->alloc(sizeof(SFParticleObject),config.maxParticleCount);
    vbo->accessData([=](void *pointer){
        auto objects = static_cast<SFParticleObject*>(pointer);
        memset(objects, 0, vbo->size);
        for (int i=0; i<vbo->count; i++) {
            for (int j=0; j<4; j++) {
                objects[i].rand[j] = rand()%1000/1000.0;
#ifdef TEST
                objects[i].tmp = 1;
                objects[i].time = 0;
                objects[i].life = 10;
#endif
            }
//#ifdef TEST
//            objects[0].tmp = 1;
//            objects[0].time = 0;
//            objects[0].life = 10;
//#endif
        }
    });
    
    
    tbo = GLBuffer::create();
    if (!config.useTriangleRenderer) {
        tbo->alloc(sizeof(SFParticlePointNode),config.maxParticleCount);
    }
    else {
        tbo->alloc(sizeof(SFParticleTriangleNode),config.maxParticleCount*4);
    }
    
    tbo->accessData([=](void *pointer){
        memset(pointer, 0, tbo->size);
    });
    
    ebo = GLBuffer::create();
    
    if (!config.useTriangleRenderer) {
        ebo->alloc(sizeof(GLuint),config.maxParticleCount);
        ebo->accessData([=](void *pointer){
            auto indexs = static_cast<GLuint*>(pointer);
            for (int i=0; i<ebo->count; i++) {
                indexs[i] = i;
            }
        });
    }
    else {
        ebo->alloc(sizeof(GLuint),config.maxParticleCount*6);
        ebo->accessData([=](void *pointer){
            auto indexs = static_cast<GLuint*>(pointer);
            memset(pointer, 0, ebo->size);
            for (int i=0; i<config.maxParticleCount; i++) {
                indexs[i*6] = i*4;
                indexs[i*6+1] = i*4+1;
                indexs[i*6+2] = i*4+2;
                
                indexs[i*6+3] = i*4;
                indexs[i*6+4] = i*4+2;
                indexs[i*6+5] = i*4+3;
            }
        });
    }
    
    
    computeVAO = GLVertexArray::create();
    computeVAO->setBuffer(GL_ARRAY_BUFFER,vbo);
    computeVAO->setBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tbo);
    computeVAO->setElementBufferType(GL_UNSIGNED_INT);
    computeVAO->setDrawMode(GL_POINTS);
    computeVAO->setParams(SFParticleObject::getLayout());
    
    renderVAO = GLVertexArray::create();
    renderVAO->setBuffer(GL_ARRAY_BUFFER,tbo);
    renderVAO->setBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    renderVAO->setElementBufferType(GL_UNSIGNED_INT);
    if (!config.useTriangleRenderer) {
        renderVAO->setDrawMode(GL_POINTS);
        renderVAO->setParams(SFParticlePointNode::getLayout());
    }
    else {
        renderVAO->setDrawMode(GL_TRIANGLES);
        renderVAO->setParams(SFParticleTriangleNode::getLayout());
    }
    
    
    
    renderProgram = GLProgram::create();
    if (!config.useTriangleRenderer) {
        renderProgram->setRenderShader(ParticlePointVertexShader, ParticlePointFragmentShader);
    }
    else {
        renderProgram->setRenderShader(ParticleTriangleVertexShader, ParticleTriangleFragmentShader);
    }
    
}

void SFParticleSystem::addParticle(string particleDescription,shared_ptr<GLTexture> texture) {
    particleTemplates.push_back(make_pair(particleDescription, texture));
    computeProgram = nullptr;
    
}
void SFParticleSystem::addEmiter(shared_ptr<SFParticleEmitter> emitter) {
    emitters.push_back(emitter);
}

void SFParticleSystem::setTransformMatrix(vector<float> matrix) {
    if (matrix.size()!=16) {
        throw length_error("matrix must be 4x4");
    };
    transformMatrix = matrix;
}

void SFParticleSystem::update(double deltaTime)
{
    if (!computeProgram) {
        stringstream ss;
        for (int i=0;i<particleTemplates.size();i++) {
            ss << "else if (tmp == "<<i+1<<".0)"<<"{"<<particleTemplates[i].first<<"}"<<endl;
        }
        
        if (!config.useTriangleRenderer) {
            computeVertexShader = ParticlePointComputeShader;
        }
        else {
            computeVertexShader = ParticleTriangleComputeShader;
        }
        auto it = computeVertexShader.find("@");
        computeVertexShader.replace(it, 1, ss.str());
        computeProgram = GLProgram::create();
        if (!config.useTriangleRenderer) {
            computeProgram->setTransformFeedbackShader(computeVertexShader,
                                                       {"type","position","size","color","textureIndex","rect","sincos"});
        }
        else {
            computeProgram->setTransformFeedbackShader(computeVertexShader,
                                                       {"type0","position0","color0","textureIndex0","uv0",
                                                        "type1","position1","color1","textureIndex1","uv1",
                                                        "type2","position2","color2","textureIndex2","uv2",
                                                        "type3","position3","color3","textureIndex3","uv3",
                                                       });
            computeProgram->setUniform("screenSize", config.screenSize.first, config.screenSize.second);
        }
    }
    computeProgram->use();
    computeProgram->setUniform("transformMatrix", transformMatrix);
    
    auto objects = static_cast<SFParticleObject*>(vbo->lock());
    auto indexs = static_cast<GLuint*>(ebo->lock());
    
    for (int i=0; i<vbo->count; i++) {
        objects[i].time += deltaTime;
    }
    
    for(auto &emiter:emitters) {
        emiter->update(this, objects, indexs,config.maxParticleCount);
    }
    ebo->unlock();
    vbo->unlock();
    
    
    computeVAO->computeUsingTransformFeedback(computeProgram);
    
    auto nodes = static_cast<SFParticleTriangleNode*>(tbo->lock());
    indexs = static_cast<GLuint*>(ebo->lock());

    ebo->unlock();
    tbo->unlock();
    
}

void SFParticleSystem::render(shared_ptr<GLFrameBuffer> framebuffer)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    framebuffer->draw(renderProgram, renderVAO, GLDrawOption());
    glFinish();
}

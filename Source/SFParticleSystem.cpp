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
    
    vbo = GLBuffer::create();
    vbo->alloc(sizeof(SFParticleObject),config.maxParticleCount);
    vbo->accessData([=](void *pointer){
        auto objects = static_cast<SFParticleObject*>(pointer);
        memset(objects, 0, vbo->size);
        for (int i=0; i<vbo->count; i++) {
            objects[i].index = i;
            for (int j=0; j<4; j++) {
                objects[i].rand[j] = rand()%1000/1000.0;
            }
#ifdef TEST
            objects[i].tmp = 1;
            objects[i].time = 0;
            objects[i].life = 10;
#endif
        }
    });
    
    tbo = GLBuffer::create();
    tbo->alloc(sizeof(SFParticleNode),config.maxParticleCount);
    tbo->accessData([=](void *pointer){
        auto nodes = static_cast<SFParticleNode*>(pointer);
        memset(nodes, 0, tbo->size);
//#ifdef TEST
//        nodes[0].color[0] = 1;
//        nodes[0].color[1] = 1;
//        nodes[0].color[2] = 1;
//        nodes[0].color[3] = 1;
//
//        nodes[0].rect[2] = 100.0/1024.0;
//        nodes[0].rect[3] = 100.0/1024.0;
//
//        nodes[0].size = 100;
//#endif

    });
    
    ebo = GLBuffer::create();
    ebo->alloc(sizeof(GLuint),config.maxParticleCount);
    ebo->accessData([=](void *pointer){
        auto indexs = static_cast<GLuint*>(pointer);
        memset(indexs, 0, ebo->size);
    });
    
    computeVAO = GLVertexArray::create();
    computeVAO->setBuffer(GL_ARRAY_BUFFER,vbo);
    //vao->setBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    computeVAO->setBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tbo);
    computeVAO->setElementBufferType(GL_UNSIGNED_INT);
    computeVAO->setDrawMode(GL_POINTS);
    computeVAO->setParams(SFParticleObject::getLayout());
    
    renderVAO = GLVertexArray::create();
    renderVAO->setBuffer(GL_ARRAY_BUFFER,tbo);
    //vao->setBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    renderVAO->setElementBufferType(GL_UNSIGNED_INT);
    renderVAO->setDrawMode(GL_POINTS);
    renderVAO->setParams(SFParticleNode::getLayout());
    
    
    renderProgram = GLProgram::create();
    renderProgram->setRenderShader(ParticleVertexShader, ParticleFragmentShader);
}

void SFParticleSystem::addParticle(string particleDescription,shared_ptr<GLTexture> texture) {
    particleTemplates.push_back(make_pair(particleDescription, texture));
    
}
void SFParticleSystem::addEmiter(shared_ptr<SFParticleEmitter> emitter) {
    emitters.push_back(emitter);
}

void SFParticleSystem::update(double deltaTime)
{
    if (!computeProgram) {
        stringstream ss;
        for (int i=0;i<particleTemplates.size();i++) {
            ss << "else if (tmp == "<<i+1<<".0)"<<"{"<<particleTemplates[i].first<<"}"<<endl;
        }
        
        computeVertexShader = ParticleComputeShader;
        auto it = computeVertexShader.find("@");
        computeVertexShader.replace(it, 1, ss.str());
        computeProgram = GLProgram::create();
        computeProgram->setTransformFeedbackShader(computeVertexShader,
        {"type","position","size","color","textureIndex","rect","sincos"});
    }
    
    computeVAO->computeUsingTransformFeedback(computeProgram);
    
    auto nodes = static_cast<SFParticleNode*>(tbo->lock());
    auto indexs = static_cast<GLuint*>(ebo->lock());
    for(auto &emiter:emitters) {
        emiter->update(this, nodes, indexs,config.maxParticleCount);
    }
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

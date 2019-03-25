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

#define TEST

using namespace Starfall;

void SFParticleSystem::setup(SFParticleConfig config)
{
    this->config = config;
    auto context = GLContext::current();
    
    tbo = GLVertexBuffer<SFParticleObject>::create();
    tbo->alloc(config.maxParticleCount);
    tbo->accessData([=](SFParticleObject *objects){
        memset(objects, 0, tbo->size);
        for (int i=0; i<tbo->count; i++) {
            objects[i].index = i;
        }
    });
    
    vbo = GLVertexBuffer<SFParticleNode>::create();
    vbo->alloc(config.maxParticleCount);
    vbo->accessData([=](SFParticleNode *nodes){
        memset(nodes, 0, vbo->size);
#ifdef TEST
        nodes[0].color[0] = 1;
        nodes[0].color[1] = 1;
        nodes[0].color[2] = 1;
        nodes[0].color[3] = 1;
        
        nodes[0].rect[2] = 100.0/1024.0;
        nodes[0].rect[3] = 100.0/1024.0;
        
        nodes[0].size = 100;
#endif
        
    });
    
    ebo = GLElementBuffer<GLuint>::create();
    ebo->alloc(config.maxParticleCount);
    ebo->accessData([=](GLuint *indexs){
        memset(indexs, 0, ebo->size);
    });
    
    vao = GLVertexArray<SFParticleNode,GLuint>::create();
    vao->setVertexBuffer(vbo);
    //vao->setElementBuffer(ebo);
    vao->setDrawMode(GL_POINTS);
    
    vao->setParams(SFParticleNode::getLayout());
    
    
    renderProgram = GLProgram::create();
    renderProgram->setShaderString(ParticleVertexShader, ParticleFragmentShader);
}

void SFParticleSystem::addParticle(string particleDescription,shared_ptr<GLTexture> texture) {
    particleTemplates.push_back(make_pair(particleDescription, texture));
}
void SFParticleSystem::addEmiter(shared_ptr<SFParticleEmitter> emitter) {
    emitters.push_back(emitter);
}

void SFParticleSystem::update(double deltaTime)
{
    auto nodes = vbo->lock();
    auto indexs = ebo->lock();
    for(auto &emiter:emitters) {
        emiter->update(this, nodes, indexs,config.maxParticleCount);
    }
    ebo->unlock();
    vbo->unlock();
    
}

void SFParticleSystem::render(shared_ptr<GLFrameBuffer> framebuffer)
{
    framebuffer->draw(renderProgram, vao);
}

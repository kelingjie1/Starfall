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

using namespace Starfall;

void SFParticleSystem::setup(SFParticleConfig config)
{
    this->config = config;
    auto context = GLContext::current();
    vbo = GLVertexBuffer<SFParticleNode>::create();
    vbo->alloc(config.maxParticleCount);
    vbo->accessData([=](SFParticleNode *nodes){
        memset(nodes, 0, vbo->size);
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

void SFParticleSystem::render()
{
    
}

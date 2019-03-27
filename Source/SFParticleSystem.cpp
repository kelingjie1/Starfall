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
    
    tbo = GLBuffer::create(GL_TRANSFORM_FEEDBACK_BUFFER);
    tbo->alloc(sizeof(SFParticleObject),config.maxParticleCount);
    tbo->accessData([=](void *pointer){
        auto objects = static_cast<SFParticleObject*>(pointer);
        memset(objects, 0, tbo->size);
        for (int i=0; i<tbo->count; i++) {
            objects[i].index = i;
        }
    });
    
    vbo = GLBuffer::create(GL_ARRAY_BUFFER);
    vbo->alloc(sizeof(SFParticleNode),config.maxParticleCount);
    vbo->accessData([=](void *pointer){
        auto nodes = static_cast<SFParticleNode*>(pointer);
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
    
    ebo = GLBuffer::create(GL_ELEMENT_ARRAY_BUFFER);
    ebo->alloc(sizeof(GLuint),config.maxParticleCount);
    ebo->accessData([=](void *pointer){
        auto indexs = static_cast<GLuint*>(pointer);
        memset(indexs, 0, ebo->size);
    });
    
    vao = GLVertexArray::create();
    vao->setBuffer(vbo);
    //vao->setBuffer(ebo);
    vao->setElementBufferType(GL_UNSIGNED_INT);
    vao->setDrawMode(GL_POINTS);
    
    vao->setParams(SFParticleNode::getLayout());
    
    
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
            ss << "else if (tmp == "<<i<<")"<<"{"<<particleTemplates[i].first<<"}"<<endl;
        }
        
        computeVertexShader = ParticleComputeShader;
        auto it = computeVertexShader.find("@");
        computeVertexShader.replace(it, 1, ss.str());
        computeProgram = GLProgram::create();
        computeProgram->setTransformFeedbackShader(computeVertexShader,
        {"type","position","size","color","textureIndex","rotation","rect"});
    }
    auto nodes = static_cast<SFParticleNode*>(vbo->lock());
    auto indexs = static_cast<GLuint*>(ebo->lock());
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

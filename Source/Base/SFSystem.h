//
//  Starfall.h
//  Starfall
//
//  Created by lingtonke on 2019/3/19.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once

#include <string>
#include <memory>
#include <sstream>
#include <ObjectiveGL/ObjectiveGL.h>
#include "SFNode.h"
#include "SFShader.h"
#include "SFEmitter.h"

#include <chrono>

namespace Starfall {
    using namespace std;
    using namespace ObjectiveGL;
    class SFConfig
    {
    public:
        int maxParticleCount = 10000;
        float pointRenderScale = 4.0f;
        pair<int, int> screenSize;
        bool usePointRenderer = false;
        bool useDefferredRendering = false;
        bool sort = false;

    };
    
    class SFSystemContext
    {
    protected:
        int lastSearchIndex;
    public:
        float deltaTime;
        int count;
        SFObject *objects;
        GLuint *indexes;
        
        int getUnusedIndex() {
            for (int i=lastSearchIndex+1; i<count; i++) {
                if (objects[i].time>=objects[i].life) {
                    lastSearchIndex = i;
                    return i;
                }
            }
            
            for (int i=0; i<lastSearchIndex; i++) {
                if (objects[i].time>=objects[i].life) {
                    lastSearchIndex = i;
                    return i;
                }
            }
            return -1;
        }
    };
    
    class SFMonitor {
    protected:
        chrono::steady_clock::time_point updateStartTime;
        chrono::steady_clock::time_point renderStartTime;
    public:
        int particleCount;
        float updateCost;
        float renderCost;
        void startUpdate() {
            updateStartTime = chrono::steady_clock::now();
        }
        
        void endUpdate() {
            auto now = chrono::steady_clock::now();
            updateCost = chrono::duration_cast<chrono::microseconds>(now-updateStartTime).count()/1000000.0f;
        }
        
        void startRender() {
            renderStartTime = chrono::steady_clock::now();
        }
        
        void endRender() {
            auto now = chrono::steady_clock::now();
            renderCost = chrono::duration_cast<chrono::microseconds>(now-updateStartTime).count()/1000000.0f;
        }
    };
    
    class SFSystem
    {
    protected:
        SFConfig config;
        shared_ptr<GLBuffer> vbo;
        shared_ptr<GLBuffer> tbo;
        shared_ptr<GLBuffer> ebo;
        shared_ptr<GLBuffer> defferredVBO;
        
        shared_ptr<GLVertexArray> computeVAO;
        shared_ptr<GLVertexArray> renderVAO;
        shared_ptr<GLVertexArray> defferredVAO;
        
        shared_ptr<GLProgram> computeProgram;
        shared_ptr<GLProgram> renderProgram;
        shared_ptr<GLProgram> defferredProgram;
        
        GLDrawOption drawOption;
        
        shared_ptr<GLTexture> defferredTexture;
        shared_ptr<GLFrameBuffer> defferredFramebuffer;
        
        string computeVertexShader;
        vector<shared_ptr<SFEmitter>> emitters;
        vector<pair<string,shared_ptr<GLTexture>>> particleTemplates;
        vector<float> transformMatrix;
        
        SFMonitor monitor;
        
        SFSystemContext context;
    public:
        SFPointNode *getNextUnusedNode();
        void setup(SFConfig config) {
            this->config = config;
            this->context.count = config.maxParticleCount;
            auto context = GLContext::current();
            
            transformMatrix =
            {1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1,
            };
            
            vbo = GLBuffer::create();
            vbo->alloc(sizeof(SFObject),config.maxParticleCount);
            vbo->accessData([=](void *pointer){
                auto objects = static_cast<SFObject*>(pointer);
                objects[0].reset();
                memset(objects, 0, (size_t)this->vbo->size);
            });
            
            
            tbo = GLBuffer::create();
            if (config.usePointRenderer) {
                tbo->alloc(sizeof(SFPointNode),config.maxParticleCount);
            }
            else {
                tbo->alloc(sizeof(SFTriangleNode),config.maxParticleCount*4);
            }
            
            tbo->accessData([=](void *pointer){
                memset(pointer, 0, this->tbo->size);
            });
            
            ebo = GLBuffer::create();
            
            if (config.usePointRenderer) {
                ebo->alloc(sizeof(GLuint),config.maxParticleCount);
                ebo->accessData([=](void *pointer){
                    auto indexs = static_cast<GLuint*>(pointer);
                    for (int i=0; i<this->ebo->count; i++) {
                        indexs[i] = i;
                    }
                });
            }
            else {
                ebo->alloc(sizeof(GLuint),config.maxParticleCount*6);
                ebo->accessData([=](void *pointer){
                    auto indexs = static_cast<GLuint*>(pointer);
                    memset(pointer, 0, (size_t)this->ebo->size);
                    for (unsigned int i=0; i<(unsigned int)config.maxParticleCount; i++) {
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
            computeVAO->setParams(SFObject::getLayout());
            
            renderVAO = GLVertexArray::create();
            renderVAO->setBuffer(GL_ARRAY_BUFFER,tbo);
            renderVAO->setBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
            renderVAO->setElementBufferType(GL_UNSIGNED_INT);
            if (config.usePointRenderer) {
                renderVAO->setDrawMode(GL_POINTS);
                renderVAO->setParams(SFPointNode::getLayout());
            }
            else {
                renderVAO->setDrawMode(GL_TRIANGLES);
                renderVAO->setParams(SFTriangleNode::getLayout());
            }
            
            
            
            renderProgram = GLProgram::create();
            if (config.usePointRenderer) {
                renderProgram->setRenderShader(ParticlePointVertexShader, ParticlePointFragmentShader);
                renderProgram->setUniform("scale", config.pointRenderScale);
            }
            else {
                renderProgram->setRenderShader(ParticleTriangleVertexShader, ParticleTriangleFragmentShader);
            }
            if (config.useDefferredRendering) {
                renderProgram->setUniform("useDefferredRendering", 1.0f);
                drawOption.enableBlend = false;
            }
            else {
                renderProgram->setUniform("useDefferredRendering", 0.0f);
                drawOption.enableBlend = true;
            }
            
            
            if (config.useDefferredRendering) {
                defferredVBO = GLBuffer::create();
                defferredVBO->alloc(sizeof(SFDefferredVertex),4);
                defferredVBO->accessData([=](void *pointer){
                    auto vertex = static_cast<SFDefferredVertex*>(pointer);
                    vertex[0].position[0] = -1;
                    vertex[0].position[1] = 1;
                    vertex[0].uv[0] = 0;
                    vertex[0].uv[1] = 1;
                    
                    vertex[1].position[0] = 1;
                    vertex[1].position[1] = 1;
                    vertex[1].uv[0] = 1;
                    vertex[1].uv[1] = 1;
                    
                    vertex[2].position[0] = 1;
                    vertex[2].position[1] = -1;
                    vertex[2].uv[0] = 1;
                    vertex[2].uv[1] = 0;
                    
                    vertex[3].position[0] = -1;
                    vertex[3].position[1] = -1;
                    vertex[3].uv[0] = 0;
                    vertex[3].uv[1] = 0;
                });
                
                
                defferredVAO = GLVertexArray::create();
                defferredVAO->setBuffer(GL_ARRAY_BUFFER,defferredVBO);
                defferredVAO->setDrawMode(GL_TRIANGLE_FAN);
                defferredVAO->setParams(SFDefferredVertex::getLayout());
                
                defferredTexture = GLTexture::create();
                defferredTexture->setImageData(nullptr, config.screenSize.first, config.screenSize.second);
                
                defferredFramebuffer = GLFrameBuffer::create();
                defferredFramebuffer->setColorTexture(defferredTexture);
                
                defferredProgram = GLProgram::create();
                defferredProgram->setRenderShader(ParticleDefferredVertexShader, ParticleDefferredFragmentShader);
                
                defferredProgram->setTexture("defferredTexture", defferredTexture);
            }
        }
        
        void addParticle(string particleDescription,shared_ptr<GLTexture> texture) {
            particleTemplates.push_back(make_pair(particleDescription, texture));
            computeProgram = nullptr;
        }
        
        void addEmiter(shared_ptr<SFEmitter> emitter) {
            emitters.push_back(emitter);
        }
        
        void setTransformMatrix(vector<float> matrix) {
            if (matrix.size()!=16) {
                throw length_error("matrix must be 4x4");
            };
            transformMatrix = matrix;
        }
        
        const SFMonitor *getMonitor() {
            return &monitor;
        }
        
        void update(double deltaTime) {
            
            monitor.startUpdate();
            
            context.deltaTime = deltaTime;
            context.objects = static_cast<SFObject*>(vbo->lock());
            context.indexes = static_cast<GLuint*>(ebo->lock());
            
            if (!computeProgram) {
                stringstream ss;
                vector<shared_ptr<GLTexture>> textures;
                textures.resize(8);
                for (int i=0;i<(int)particleTemplates.size();i++) {
                    ss << "else if (tmp == "<<i+1<<".0)"<<"{"<<particleTemplates[i].first<<"}"<<endl;
                    textures[i] = particleTemplates[i].second;
                }
                
                if (config.usePointRenderer) {
                    computeVertexShader = ParticlePointComputeShader;
                }
                else {
                    computeVertexShader = ParticleTriangleComputeShader;
                }
                auto it = computeVertexShader.find("@");
                computeVertexShader.replace(it, 1, ss.str());
                computeProgram = GLProgram::create();
                if (config.usePointRenderer) {
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
                    computeProgram->setUniform("screenSize", (float)config.screenSize.first, (float)config.screenSize.second);
                }
                
                renderProgram->setTextures("textures", textures);
                if (config.useDefferredRendering) {
                    defferredProgram->setTextures("textures", textures);
                }
                
                
                for(auto &emiter:emitters) {
                    emiter->init(&context);
                }
                
                
            }
            else {
                int count = 0;
                for (int i=0; i<vbo->count; i++) {
                    auto &obj = context.objects[i];
                    obj.time += deltaTime;
                    if (obj.time>0 && obj.time<obj.life) {
                        count++;
                    }
                }
                monitor.particleCount = count;
                
                for(auto &emiter:emitters) {
                    emiter->update(&context);
                }
            }
            
            ebo->unlock();
            vbo->unlock();
            computeProgram->setUniformMatrix("transformMatrix", transformMatrix);
            
            
            
            
            computeVAO->computeUsingTransformFeedback(computeProgram);
            
            auto nodes = static_cast<SFTriangleNode*>(tbo->lock());
            auto indexs = static_cast<GLuint*>(ebo->lock());
            //
            //    int count = 0;
            //    for (int i=0; i<vbo->count; i++) {
            //        if (nodes[i].textureIndex>0.5) {
            //            indexs[count*6] = i*4;
            //            indexs[count*6+1] = i*4+1;
            //            indexs[count*6+2] = i*4+2;
            //
            //            indexs[count*6+3] = i*4;
            //            indexs[count*6+4] = i*4+2;
            //            indexs[count*6+5] = i*4+3;
            //            count++;
            //        }
            //    }
            //    renderVAO->setDrawCount(count*6);
            ebo->unlock();
            tbo->unlock();
            
            //glFinish();
            monitor.endUpdate();
        }
        
        void render(shared_ptr<GLFrameBuffer> framebuffer) {
            
            monitor.startRender();
            
            if (!config.useDefferredRendering) {
                framebuffer->draw(renderProgram, renderVAO, drawOption);
            }
            else {
                defferredFramebuffer->clear(0, 0, 0, 1);
                defferredFramebuffer->draw(renderProgram, renderVAO, drawOption);
                framebuffer->draw(defferredProgram, defferredVAO, drawOption);
            }
            
            //glFinish();
            monitor.endRender();
        }
    };
}


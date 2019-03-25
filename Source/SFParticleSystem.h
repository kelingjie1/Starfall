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
#include "ObjectiveGL.h"
#include "SFParticleNode.h"
#include "SFParticleEmitter.h"

namespace Starfall {
    using namespace std;
    using namespace ObjectiveGL;
    class SFParticleConfig
    {
    public:
        int maxParticleCount;
    };
    class SFParticleSystem
    {
    protected:
        SFParticleConfig config;
        shared_ptr<GLVertexBuffer<SFParticleObject>> tbo;
        shared_ptr<GLVertexBuffer<SFParticleNode>> vbo;
        shared_ptr<GLElementBuffer<GLuint>> ebo;
        shared_ptr<GLVertexArray<SFParticleNode, GLuint>> vao;
        shared_ptr<GLProgram> computeProgram;
        shared_ptr<GLProgram> renderProgram;
        string computeVertexShader;
        vector<shared_ptr<SFParticleEmitter>> emitters;
        vector<pair<string,shared_ptr<GLTexture>>> particleTemplates;
        
    public:
        SFParticleNode *getNextUnusedNode();
        void setup(SFParticleConfig config);
        void addParticle(string particleDescription,shared_ptr<GLTexture> texture);
        void addEmiter(shared_ptr<SFParticleEmitter> emitter);
        void update(double deltaTime);
        void render(shared_ptr<GLFrameBuffer> framebuffer);
    };
}


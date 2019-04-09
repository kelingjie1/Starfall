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
        bool useTriangleRenderer;
        bool useDefferredRendering;
        pair<GLfloat, GLfloat> screenSize;
    };
    class SFParticleSystem
    {
    protected:
        SFParticleConfig config;
        shared_ptr<GLBuffer> vbo;
        shared_ptr<GLBuffer> tbo;
        shared_ptr<GLBuffer> ebo;
        shared_ptr<GLVertexArray> computeVAO;
        shared_ptr<GLVertexArray> renderVAO;
        shared_ptr<GLProgram> computeProgram;
        shared_ptr<GLProgram> renderProgram;
        string computeVertexShader;
        vector<shared_ptr<SFParticleEmitter>> emitters;
        vector<pair<string,shared_ptr<GLTexture>>> particleTemplates;
        vector<float> transformMatrix;
        
    public:
        SFParticlePointNode *getNextUnusedNode();
        void setup(SFParticleConfig config);
        void addParticle(string particleDescription,shared_ptr<GLTexture> texture);
        void addEmiter(shared_ptr<SFParticleEmitter> emitter);
        void setTransformMatrix(vector<float> matrix);
        void update(double deltaTime);
        void render(shared_ptr<GLFrameBuffer> framebuffer);
    };
}


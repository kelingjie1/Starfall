//
//  SFParticleNode.hpp
//  Starfall
//
//  Created by lingtonke on 2019/3/20.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include "GLVertexArray.h"
#include <vector>

namespace Starfall
{
    using namespace std;
    using namespace ObjectiveGL;
    class SFParticleObject {
    public:
        GLfloat index;
        GLfloat tmp;
        GLfloat time;
        GLfloat life;
        GLfloat rand[4];
        GLfloat frameSize[2];
        GLfloat frameIndex;
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            return params;
        }
    };
    class SFParticlePointNode
    {
    public:
        GLfloat type;
        GLfloat position[4];
        GLfloat size;
        GLfloat color[4];
        GLfloat textureIndex;
        GLfloat rect[4];
        GLfloat sincos[2];
        
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            return params;
        }
    };
    
    class SFParticleTriangleNode
    {
    public:
        GLfloat type;
        GLfloat position[4];
        GLfloat color[4];
        GLfloat textureIndex;
        GLfloat uv[2];
        
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            return params;
        }
    };
}

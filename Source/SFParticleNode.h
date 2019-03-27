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
        GLint index;
        GLint tmp;
        GLfloat time;
        GLfloat life;
        GLfloat rand[4];
        GLfloat frameSize[2];
        GLfloat frameIndex;
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_INT));
            params.push_back(GLVertexArrayParams(GL_INT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            return params;
        }
    };
    class SFParticleNode
    {
    public:
        GLint type;//0:死亡/1:初始化/2:活着
        GLfloat position[3];
        GLfloat size;
        GLfloat color[4];
        GLfloat textureIndex;
        GLfloat rotation;
        GLfloat rect[4];
        
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_INT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,3));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            return params;
        }
    };
}

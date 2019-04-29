//
//  SFNode.hpp
//  Starfall
//
//  Created by lingtonke on 2019/3/20.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include <ObjectiveGL/ObjectiveGL.h>
#include <vector>

namespace Starfall
{
    using namespace std;
    using namespace ObjectiveGL;
    class SFObject {
    public:
        GLfloat tmp;
        GLfloat time;
        GLfloat life;
        GLfloat frameIndex;
        GLfloat rand[4];
        GLfloat frameSize[2];
        void reset() {
            for (int j=0; j<4; j++) {
                rand[j] = std::rand()%1000/1000.0;
            }
            tmp = 1;
            time = 0;
            life = 999999;
        }
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT));
            params.push_back(GLVertexArrayParams(GL_FLOAT,4));
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            return params;
        }
    };
    class SFPointNode
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
    
    class SFTriangleNode
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
    
    class SFDefferredVertex {
    public:
        GLfloat position[2];
        GLfloat uv[2];
        
        static vector<GLVertexArrayParams> getLayout() {
            vector<GLVertexArrayParams> params;
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            params.push_back(GLVertexArrayParams(GL_FLOAT,2));
            return params;
        }
    };
}

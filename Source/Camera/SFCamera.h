//
//  SFCamera.hpp
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include <vector>
namespace Starfall {
    using namespace std;
    class SFCamera
    {
    protected:
        bool changed;
        vector<float> vpMatrix;
        vector<float> viewMatrix;
        vector<float> projectionMatrix;
        
        float posX;
        float posY;
        float posZ;
        
        float lookAtX;
        float lookAtY;
        float lookAtZ;
        
        float upX;
        float upY;
        float upZ;
        
        SFCamera();
    public:
        vector<float> getMatrix();
        void setPerspective(float fovy, float aspect, float zNear, float zFar);
        void setPosition(float x,float y,float z);
        void setLookAt(float x,float y,float z);
        
        void setPositionOrbit(float rotationX,float rotationY,float distance);
    };
}

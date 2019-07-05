//
//  SFCamera.hpp
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include <vector>
#include <memory>
namespace Starfall {
    using namespace std;
    class SFCamera
    {
    protected:
        bool changed;
        vector<float> vpMatrix;
        vector<float> viewMatrix;
        vector<float> projectionMatrix;
        
        float rotationX;
        float rotationY;
        float distance;
        
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
        void setPosition(float x,float y,float z);
    public:
        static shared_ptr<SFCamera> create();
        vector<float> getVPMatrix();
        void setPerspective(float fovy, float aspect, float zNear, float zFar);
        void setLookAt(float x,float y,float z);
        void setPositionOrbit(float rotationX,float rotationY,float distance);
        
        float getRotationX();
        float getRotationY();
        float getDistance();
    };
}

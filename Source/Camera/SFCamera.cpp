//
//  SFCamera.cpp
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#include "SFCamera.h"
#import <glm/glm.hpp>
#import <glm/ext/matrix_transform.hpp>
#import <glm/ext/matrix_clip_space.hpp>
#import <glm/ext/scalar_constants.hpp>
#import <glm/gtc/type_ptr.hpp>
using namespace Starfall;
using namespace glm;

SFCamera::SFCamera():lookAtX(0),lookAtY(0),lookAtZ(0),posX(0),posY(0),posZ(100),upX(0),upY(1),upZ(0) {

}

void SFCamera::setPerspective(float fovy, float aspect, float zNear, float zFar) {
    changed = true;
    auto matrix = perspective(fovy, aspect, zNear, zFar);
    auto ptr = value_ptr(matrix);
    projectionMatrix = vector<float>(ptr,ptr+16);
    
}

vector<float> SFCamera::getMatrix() {
    if (changed) {
        
        mat4 proj = make_mat4(projectionMatrix.data());
        mat4 view = lookAt(vec3(lookAtX,lookAtY,lookAtZ), vec3(posX,posY,posZ), vec3(upX,upY,upZ));
        auto matrix = proj*view;
        auto ptr = value_ptr(matrix);
        vpMatrix = vector<float>(ptr,ptr+16);
    }
    return vpMatrix;
}

void SFCamera::setPosition(float x,float y,float z) {
    changed = true;
    posX = x;
    posY = y;
    posZ = z;
}

void SFCamera::setLookAt(float x,float y,float z) {
    changed = true;
    lookAtX = x;
    lookAtY = y;
    lookAtZ = z;
}

void SFCamera::setPositionOrbit(float rotationX, float rotationY, float distance) {
    vec4 v(0,0,1,0);
    mat4 m(1);
    m = rotate(m, rotationX, glm::vec3(0,1,0));
    m = rotate(m, rotationY, glm::vec3(1,0,0));
    v = m*v;
    v *= distance;
    
    v+= vec4(lookAtX,lookAtY,lookAtZ,0);
    setPosition(v.x, v.y, v.z);
}
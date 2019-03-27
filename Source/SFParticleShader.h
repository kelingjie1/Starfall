//
//  SFShader.h
//  Starfall
//
//  Created by lingtonke on 2019/3/22.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once

#include "GLProgram.h"
#include <string>

namespace Starfall {
    using namespace std;
    const string ParticleComputeShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in float index;
     layout(location = 1) in float tmp;
     layout(location = 2) in float time;
     layout(location = 3) in float life;
     layout(location = 4) in vec4 rand;
     layout(location = 5) in vec2 frameSize;
     layout(location = 6) in float frameIndex;
     
     
     out float type;
     out vec3 position;
     out float size;
     out vec4 color;
     out float textureIndex;
     out float rotation;
     out vec4 rect;
     
     void main()
     {
//         type = 1.0;
//         position = vec3(0.0);
//         size = 0.0;
//         color = vec4(1.0);
//         textureIndex = 1.0;
//         rotation = 0.0;
//         rect = vec4(0.0);
         
         type = 0.0;
         position = vec3(0.0);
         size = 0.0;
         color = vec4(1.0);
         textureIndex = 1.0;
         rotation = 0.0;
         rect = vec4(0.0,0.0,100.0/1024.0,100.0/1024.0);
         if (time>=life) {
             type = 0.0;
             return;
         }
         @
     }
     );
    
    const string ParticleVertexShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in float type;//0:死亡/1:初始化/2:活着
     layout(location = 1) in vec3 position;
     layout(location = 2) in float size;
     layout(location = 3) in vec4 color;
     layout(location = 4) in float textureIndex;
     layout(location = 5) in float rotation;
     layout(location = 6) in vec4 rect;

     out float fs_textureIndex;
     out vec4 fs_color;
     out float fs_rotation;
     out vec4 fs_rect;
     
     uniform mat4 vpMatrix;
     uniform vec2 screenSize;
     
     
     void main()
     {
         if (size == 0.0) {
             gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
             gl_PointSize = 0.0;
             fs_textureIndex = -1.0;
             return;
         }
         gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
         gl_PointSize = 1000.0;
         //gl_Position = vpMatrix*vec4(position.x, position.y, position.z, 1.0);
         //gl_PointSize = screenSize.x/gl_Position.w*size*1.414;
         
         fs_textureIndex = textureIndex;
         fs_color = color;
         fs_rotation = rotation;
         fs_rect = rect;
     }
     );
    
    const string ParticleFragmentShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     precision highp float;
     layout(location = 0) out vec4 color;
     uniform sampler2D textures[8];
     uniform int premultiplied[8];
     uniform float radian;
     
     in float fs_textureIndex;
     in vec4 fs_color;
     in float fs_rotation;
     in vec4 fs_rect;
     void main()
     {
         if (fs_textureIndex < 0.0) {
             discard;
             return;
         }
         float r = radian + fs_rotation;
         vec2 pos = vec2(gl_PointCoord.x-0.5,0.5-gl_PointCoord.y);
         pos = vec2((cos(r)*pos.x+sin(r)*pos.y)*1.414+0.5,
                    (cos(r)*pos.y-sin(r)*pos.x)*1.414+0.5);
         if (pos.x<0.0||pos.x>1.0||pos.y<0.0||pos.y>1.0)
         {
             discard;
         }
         vec4 texColor = vec4(1.0,1.0,1.0,1.0);
         texColor = texture(textures[0], vec2(fs_rect.x+pos.x*fs_rect.z,
                                              1.0-(fs_rect.y+pos.y*fs_rect.w)));
         texColor = vec4(texColor.r * fs_color.r * fs_color.a,
                         texColor.g * fs_color.g * fs_color.a,
                         texColor.b * fs_color.b * fs_color.a,
                         texColor.a * fs_color.a);
         color = texColor;
     }
     );
    
}


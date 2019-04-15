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
    const string ParticlePointComputeShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in float tmp;
     layout(location = 1) in float time;
     layout(location = 2) in float life;
     layout(location = 3) in float frameIndex;
     layout(location = 4) in vec4 rand;
     layout(location = 5) in vec2 frameSize;

     
     uniform mat4 transformMatrix;
     
     float rotation;
     
     out float type;
     out vec4 position;
     out float size;
     out vec4 color;
     out float textureIndex;
     out vec4 rect;
     out vec2 sincos;
     
     void main()
     {
         type = 0.0;
         position = vec4(0.0);
         size = 0.0;
         color = vec4(1.0);
         textureIndex = 1.0;
         rotation = 0.0;
         rect = vec4(0.0,0.0,100.0/1024.0,100.0/1024.0);
         if (time>=life) {
             type = 0.0;
         }
         @
         else {
             position = transformMatrix*position;
         }
         sincos = vec2(sin(-rotation),cos(-rotation));
     }
     );
    
    const string ParticlePointVertexShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in float type;//0:死亡/1:初始化/2:活着
     layout(location = 1) in vec4 position;
     layout(location = 2) in float size;
     layout(location = 3) in vec4 color;
     layout(location = 4) in float textureIndex;
     layout(location = 5) in vec4 rect;
     layout(location = 6) in vec2 sincos;

     out vec2 fs_sincos;
     out float fs_textureIndex;
     out vec4 fs_color;
     out vec4 fs_rect;
     
     
     void main()
     {
         if (size == 0.0) {
             gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
             gl_PointSize = 0.0;
             fs_textureIndex = -1.0;
             return;
         }
         gl_Position = position;
         gl_PointSize = size/position.w/2.0*1.414;
         
         fs_sincos = sincos;
         fs_textureIndex = textureIndex;
         fs_color = color;
         fs_rect = rect;
     }
     );
    
    const string ParticlePointFragmentShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     precision lowp float;
     layout(location = 0) out vec4 color;
     uniform sampler2D textures[8];
     uniform float premultiplied[8];
     uniform float useDefferredRendering;
     
     in vec2 fs_sincos;
     in float fs_textureIndex;
     in vec4 fs_color;
     in vec4 fs_rect;
     
     void main()
     {
         if (fs_textureIndex < 0.0) {
             discard;
             return;
         }
         vec2 pos = vec2(gl_PointCoord.x-0.5,0.5-gl_PointCoord.y);
         pos = vec2(fs_sincos.y*pos.x+fs_sincos.x*pos.y,
                    fs_sincos.y*pos.y-fs_sincos.x*pos.x);
         pos*= 1.414;
         pos = pos+vec2(0.5,0.5);
         if (pos.x<0.0||pos.x>1.0||pos.y<0.0||pos.y>1.0)
         {
             discard;
         }
         vec2 uv = vec2(fs_rect.x+pos.x*fs_rect.z,
                        1.0-(fs_rect.y+pos.y*fs_rect.w));
         if (useDefferredRendering<0.5) {
             vec4 texColor = vec4(1.0,1.0,1.0,1.0);
             texColor = texture(textures[0], uv);
             if (texColor.a<0.1) {
                 discard;
                 return;
             }
             color = vec4(texColor.r * fs_color.r * fs_color.a,
                          texColor.g * fs_color.g * fs_color.a,
                          texColor.b * fs_color.b * fs_color.a,
                          texColor.a * fs_color.a);
         } else {
             color = vec4(fs_textureIndex,uv,1.0);
         }
     }
     );
    
    
    
    
    const string ParticleTriangleComputeShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in float tmp;
     layout(location = 1) in float time;
     layout(location = 2) in float life;
     layout(location = 3) in float frameIndex;
     layout(location = 4) in vec4 rand;
     layout(location = 5) in vec2 frameSize;
     
     
     
     uniform vec2 screenSize;
     uniform mat4 transformMatrix;
     
     const float PI = 3.1415926;
     const float PI_2 = 1.5707963;
     
     float rotation;
     float type;
     vec4 position;
     float size;
     vec4 color;
     float textureIndex;
     vec4 rect;
     
     out float type0;
     out vec4 position0;
     out vec4 color0;
     out float textureIndex0;
     out vec2 uv0;
     
     out float type1;
     out vec4 position1;
     out vec4 color1;
     out float textureIndex1;
     out vec2 uv1;
     
     out float type2;
     out vec4 position2;
     out vec4 color2;
     out float textureIndex2;
     out vec2 uv2;
     
     out float type3;
     out vec4 position3;
     out vec4 color3;
     out float textureIndex3;
     out vec2 uv3;
     
     vec2 rotate(vec2 pos,float rotation) {
         return vec2(cos(rotation)*pos.x-sin(rotation)*pos.y,
                       sin(rotation)*pos.x+cos(rotation)*pos.y);
     }
     
     void main()
     {
         position0 = vec4(0,0,0.0,0.0);
         position1 = vec4(0,0,0.0,0.0);
         position2 = vec4(0,0,0.0,0.0);
         position3 = vec4(0,0,0.0,0.0);
         
         type = 0.0;
         position = vec4(0.0);
         size = 0.0;
         color = vec4(1.0);
         textureIndex = 1.0;
         rotation = 0.0;
         rect = vec4(0.0,0.0,1.0,1.0);
         if (time>=life) {
             type = 0.0;
             return;
         }
         @
         else {
             position = transformMatrix*position;
         }
         
         
         type0 = type;
         type1 = type;
         type2 = type;
         type3 = type;
         
         float of = size/position.w/2.0;
         vec2 offset = vec2(of/screenSize.x,of/screenSize.y);
         position0 = position+vec4(rotate(offset,-PI_2-rotation),0.0,0.0);
         position1 = position+vec4(rotate(offset,-rotation),0.0,0.0);
         position2 = position+vec4(rotate(offset,PI_2-rotation),0.0,0.0);
         position3 = position+vec4(rotate(offset,PI-rotation),0.0,0.0);
         
         color0 = color;
         color1 = color;
         color2 = color;
         color3 = color;
         
         textureIndex0 = textureIndex;
         textureIndex1 = textureIndex;
         textureIndex2 = textureIndex;
         textureIndex3 = textureIndex;
         
         uv0 = vec2(rect.x,1.0-(rect.y+rect.w));
         uv1 = vec2(rect.x+rect.z,1.0-(rect.y+rect.w));
         uv2 = vec2(rect.x+rect.z,1.0-rect.y);
         uv3 = vec2(rect.x,1.0-rect.y);
         
     }
     );
    
    const string ParticleTriangleVertexShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in float type;
     layout(location = 1) in vec4 position;
     layout(location = 2) in vec4 color;
     layout(location = 3) in float textureIndex;
     layout(location = 4) in vec2 uv;
     
     out float fs_textureIndex;
     out vec4 fs_color;
     out vec2 fs_uv;
     
     void main()
     {
         if (textureIndex < 0.0) {
             gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
             return;
         }
         gl_Position = position;
         
         fs_textureIndex = textureIndex;
         fs_color = color;
         fs_uv = uv;
     }

     );
    
    const string ParticleTriangleFragmentShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     precision lowp float;
     layout(location = 0) out vec4 color;
     uniform sampler2D textures[8];
     uniform float premultiplied[8];
     uniform float useDefferredRendering;
     
     in float fs_textureIndex;
     in vec4 fs_color;
     in vec2 fs_uv;
     
     void main()
     {
         if (fs_textureIndex < 0.0) {
             discard;
             return;
         }
         if (useDefferredRendering<0.5) {
             vec4 texColor = vec4(1.0,1.0,1.0,1.0);
             texColor = texture(textures[0], fs_uv);
             if (texColor.a<0.1) {
                 discard;
                 return;
             }
             color = vec4(texColor.r * fs_color.r * fs_color.a,
                          texColor.g * fs_color.g * fs_color.a,
                          texColor.b * fs_color.b * fs_color.a,
                          texColor.a * fs_color.a);
         } else {
             color = vec4(fs_textureIndex,fs_uv,1.0);
         }
         
     }
     );
    
    const string ParticleDefferredVertexShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in vec4 position;
     layout(location = 1) in vec2 uv;
     
     out vec2 fs_uv;
     
     void main()
     {
         gl_Position = position;
         fs_uv = uv;
     }
     
     );
    
    const string ParticleDefferredFragmentShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     precision lowp float;
     layout(location = 0) out vec4 color;
     uniform sampler2D defferredTexture;
     uniform sampler2D textures[8];
     uniform float premultiplied[8];
     
     in vec2 fs_uv;
     
     void main()
     {
         vec4 texColor = texture(defferredTexture, fs_uv);
         if (texColor.r<0.5) {
             discard;
             return;
         }
         
         color = texture(textures[0],texColor.gb);
         
     }
     );
    
}


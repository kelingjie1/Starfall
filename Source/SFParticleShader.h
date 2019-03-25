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
    const string ParticleVertexShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     layout(location = 0) in uint index;
     layout(location = 1) in vec3 position;
     layout(location = 2) in vec2 pointSize;
     layout(location = 3) in vec4 vs_colorFactor;
     layout(location = 4) in vec3 vs_texCoord;
     layout(location = 5) in float vs_rotation;
     layout(location = 6) in float vs_frameIndex;
     
     out vec4 fs_colorFactor;
     out vec3 fs_texCoord;
     out float fs_rotation;
     out vec2 fs_pointScale;
     
     uniform mat4 vpMatrix;
     uniform vec2 screenSize;
     
     
     void main()
     {
         gl_Position = vpMatrix*vec4(position.x, position.y, position.z, 1.0);
         float longEdge = max(pointSize.x,pointSize.y);
         gl_PointSize = screenSize.x/gl_Position.w*longEdge*1.414;
         fs_pointScale = vec2(pointSize.x/longEdge,pointSize.y/longEdge);
         
         fs_colorFactor = vs_colorFactor;
         fs_texCoord = vs_texCoord;
         fs_rotation = vs_rotation;
     }
     );
    
    const string ParticleFragmentShader = string("#version 300 es\n")+
    SHADER_STRING
    (
     precision highp float;
     layout(location = 0) out vec4 color;
     uniform sampler2D textures[8];
     uniform int premultiplied[8];
     uniform vec2 frameSizes[8];
     uniform float radian;
     
     in vec3 fs_texCoord;
     in vec4 fs_colorFactor;
     in float fs_rotation;
     in vec2 fs_pointScale;
     
     
     
     void main()
     {
         float r = radian + fs_rotation;
         vec2 pos = vec2(gl_PointCoord.x-0.5,0.5-gl_PointCoord.y);
         pos = vec2((cos(r)*pos.x+sin(r)*pos.y)*1.414/fs_pointScale.x+0.5,
                    (cos(r)*pos.y-sin(r)*pos.x)*1.414/fs_pointScale.y+0.5);
         if (pos.x<0.0||pos.x>1.0||pos.y<0.0||pos.y>1.0)
         {
             discard;
         }
         int index = int(fs_texCoord.z+0.5);
         vec2 frameSize = frameSizes[index];
         vec4 texColor = vec4(1.0,1.0,1.0,1.0);
         if (index==0)
         {
             texColor = texture(textures[0], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==1)
         {
             texColor = texture(textures[1], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==2)
         {
             texColor = texture(textures[2], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==3)
         {
             texColor = texture(textures[3], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==4)
         {
             texColor = texture(textures[4], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==5)
         {
             texColor = texture(textures[5], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==6)
         {
             texColor = texture(textures[6], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         else if (index==7)
         {
             texColor = texture(textures[7], vec2((fs_texCoord.x+pos.x)*frameSize.x,
                                                  (fs_texCoord.y+pos.y)*frameSize.y));
         }
         
         //         if (premultiplied[index]==1)
         //         {
         //             texColor.rgb = texColor.rgb * texColor.a;
         //         }
         texColor = vec4(texColor.r * fs_colorFactor.r * fs_colorFactor.a,
                         texColor.g * fs_colorFactor.g * fs_colorFactor.a,
                         texColor.b * fs_colorFactor.b * fs_colorFactor.a,
                         texColor.a * fs_colorFactor.a);
         color = texColor;
     }
     );
    
}


//
//  SFParser.cpp
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#include "SFParser.h"
#include <rapidjson/document.h>
#include <fstream>
#include <sstream>
#include "../Base/SFSystem.h"
#include "../Camera/SFCamera.h"
#include "../Emitter/SFFillEmitter.h"
#include "../Emitter/SFNormalEmitter.h"

using namespace Starfall;
using namespace rapidjson;

shared_ptr<SFSystem> SFParser::parsePath(string path,int screenWidth,int screenHeight,shared_ptr<SFCamera> *cameraout,function<shared_ptr<GLTexture>(string path)> textureReadFunc) {
    
    auto pos = path.find_last_of("/");
    auto dir = path.substr(0,pos);
    auto file = path.substr(pos+1,path.size()-pos-1);
    ifstream f(path);
    istreambuf_iterator<char> begin(f),end;
    string json(begin,end);
    Document d;
    d.Parse(json.c_str());
    auto &config = d["config"];
    SFConfig cfg;
    cfg.maxParticleCount = config["maxParticleCount"].GetInt();
    if (config.HasMember("usePointRenderer")) {
        cfg.usePointRenderer = config["usePointRenderer"].GetBool();
    }
    if (config.HasMember("pointRenderScale")) {
        cfg.pointRenderScale = config["pointRenderScale"].GetFloat();
    }
    if (config.HasMember("useDefferredRendering")) {
        cfg.useDefferredRendering = config["useDefferredRendering"].GetBool();
    }
    cfg.screenSize = make_pair(screenWidth, screenHeight);
    auto particleSystem = make_shared<SFSystem>();
    particleSystem->setup(cfg);
    
    auto particles = d["particles"].GetArray();
    auto textures = d["textures"].GetArray();
    assert(particles.Size()==textures.Size());
    for (int i=0; i<(int)particles.Size(); i++) {
        auto texturePath = dir+"/"+textures[i].GetString();
        auto particlePath = dir+"/"+particles[i].GetString();
        auto texture = textureReadFunc(texturePath);
        
        ifstream f(particlePath);
        istreambuf_iterator<char> begin(f),end;
        string particleStr(begin,end);
        
        particleSystem->addParticle(particleStr, texture);
    }
    auto &emitters = d["emitters"];
    for (auto &emitter:emitters.GetArray()) {
        string name = emitter["name"].GetString();
        if (name == "SFFillEmitter") {
            particleSystem->addEmiter(SFFillEmitter::create());
        }
        else if (name == "SFNormalEmitter") {
            auto e = SFNormalEmitter::create();
            e->emitRate = emitter["emitRate"].GetFloat();
            particleSystem->addEmiter(e);
        }
    }
    if (d.HasMember("camera")) {
        auto &camera = d["camera"];
        auto cam = SFCamera::create();
        if (cameraout) {
            *cameraout = cam;
        }
        cam->setPerspective(3.1415926f*0.25f, screenWidth/(float)screenHeight, 0.1f, 1000.0f);
        if (camera.HasMember("lookAt")) {
            auto &lookAt = camera["lookAt"];
            cam->setLookAt(lookAt["x"].GetFloat(), lookAt["y"].GetFloat(), lookAt["z"].GetFloat());
        }
        if (camera.HasMember("position")) {
            auto &position = camera["position"];
            string type = position["type"].GetString();
            if (type == "orbit") {
                cam->setPositionOrbit(position["rotationX"].GetFloat(), position["rotationY"].GetFloat(), position["distance"].GetFloat());
            }
        }
        particleSystem->setTransformMatrix(cam->getVPMatrix());
    }
    
    
    return particleSystem;
}

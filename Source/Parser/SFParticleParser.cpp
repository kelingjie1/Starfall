//
//  SFParticleParser.cpp
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#include "SFParticleParser.h"
#include <rapidjson/document.h>
#include <fstream>
#include <sstream>
#include "../Base/SFParticleSystem.h"

using namespace Starfall;
using namespace rapidjson;

shared_ptr<SFParticleSystem> SFParticleParser::parsePath(string path,int screenWidth,int screenHeight) {
    
    auto pos = path.find_last_of("/");
    auto dir = path.substr(0,pos);
    auto file = path.substr(pos+1,path.size()-pos-1);
    ifstream f(path);
    istreambuf_iterator<char> begin(f),end;
    string json(begin,end);
    Document d;
    d.Parse(json.c_str());
    auto &config = d["config"];
    SFParticleConfig cfg;
    cfg.maxParticleCount = config["maxParticleCount"].GetInt();
    if (config.HasMember("usePointRenderer")) {
        cfg.usePointRenderer = config["usePointRenderer"].GetBool();
    }
    if (config.HasMember("pointRenderScale")) {
        cfg.usePointRenderer = config["pointRenderScale"].GetFloat();
    }
    if (config.HasMember("useDefferredRendering")) {
        cfg.usePointRenderer = config["useDefferredRendering"].GetBool();
    }
    cfg.screenSize = make_pair(screenWidth, screenHeight);
    auto particleSystem = make_shared<SFParticleSystem>();
    particleSystem->setup(cfg);
    
    auto particles = d["particles"].GetArray();
    auto textures = d["textures"].GetArray();
    assert(particles.Size()==textures.Size());
    for (int i=0; i<particles.Size(); i++) {
        auto texturePath = dir+"/"+textures[i].GetString();
        auto particlePath = dir+"/"+particles[i].GetString();
        auto texture = GLPlatform::createTextureFromFile(texturePath);
        
        ifstream f(particlePath);
        istreambuf_iterator<char> begin(f),end;
        string particleStr(begin,end);
        
        particleSystem->addParticle(particleStr, texture);
    }
    
    auto &extra = d["extra"];
    return particleSystem;
}

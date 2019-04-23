//
//  SFParticleParser.h
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include <string>
#include <memory>
#include "../Base/SFParticleEmitter.h"

namespace Starfall {
    using namespace std;
    class SFParticleParser
    {
    public:
        static shared_ptr<SFParticleSystem> parsePath(string path,int screenWidth,int screenHeight);
    };
}



//
//  SFParticleEmitter.hpp
//  Starfall
//
//  Created by lingtonke on 2019/3/25.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include "SFParticleNode.h"

namespace Starfall {
    class SFParticleSystem;
    class SFParticleEmitter {
    public:
        virtual void update(SFParticleSystem *system, SFParticleObject *object, GLuint *indexs, int count);
    };
}

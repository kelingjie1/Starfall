//
//  SFEmitter.hpp
//  Starfall
//
//  Created by lingtonke on 2019/3/25.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include "SFNode.h"

namespace Starfall {
    class SFSystem;
    class SFEmitter {
    public:
        virtual void init(SFSystem *system, SFObject *objects, GLuint *indexs, int count) {}
        virtual void update(SFSystem *system, SFObject *objects, GLuint *indexs, int count) {}
    };
}

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
    class SFSystemContext;
    class SFEmitter {
    public:
        virtual void init(SFSystemContext *context) {}
        virtual void update(SFSystemContext *context) {}
        virtual ~SFEmitter() {}
    };
}

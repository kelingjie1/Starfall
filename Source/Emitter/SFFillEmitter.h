//
//  SFFillEmitter.h
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include <memory>
#include "../Base/SFEmitter.h"
#include "../Base/SFSystem.h"
namespace Starfall {
    using namespace std;
    class SFFillEmitter : public SFEmitter
    {
    public:
        static shared_ptr<SFFillEmitter> create() {
            return make_shared<SFFillEmitter>();
        }
        
        virtual void init(SFSystemContext *context) {
            for (int i=0; i<context->count; i++) {
                context->objects[i].reset();
            }
        }
        virtual void update(SFSystemContext *context) {
            
        }
    };
}

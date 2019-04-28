//
//  SFGravityEmitter.h
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
    class SFNormalEmitter : public SFEmitter
    {
    protected:
        float restTime;
    public:
        float emitRate;
        static shared_ptr<SFNormalEmitter> create() {
            return make_shared<SFNormalEmitter>();
        }
        
        virtual void init(SFSystemContext *context) {

        }
        virtual void update(SFSystemContext *context) {
            float duration = 1/emitRate;
            restTime += context->deltaTime;
            int count = restTime/duration;
            restTime -= count*duration;
            for (int i=0;i<count;i++) {
                auto index = context->getUnusedIndex();
                if (index>=0) {
                    context->objects[index].reset();
                    context->objects[index].life = 10;
                }
            }
            
        }
    };
}

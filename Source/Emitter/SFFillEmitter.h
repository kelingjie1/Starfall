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
namespace Starfall {
    using namespace std;
    class SFFillEmitter : public SFEmitter
    {
    public:
        static shared_ptr<SFFillEmitter> create() {
            return make_shared<SFFillEmitter>();
        }
        
        virtual void init(SFSystem *system, SFObject *objects, GLuint *indexs, int count) {
            for (int i=0; i<count; i++) {
                for (int j=0; j<4; j++) {
                    objects[i].rand[j] = rand()%1000/1000.0;
                    objects[i].tmp = 1;
                    objects[i].time = 0;
                    objects[i].life = 999999;
                }
            }
        }
        virtual void update(SFSystem *system, SFObject *objects, GLuint *indexs, int count) {
            
        }
    };
}

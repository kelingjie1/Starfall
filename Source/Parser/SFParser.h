//
//  SFParser.h
//  Starfall
//
//  Created by lingtonke on 2019/4/23.
//  Copyright © 2019 Starfall. All rights reserved.
//

#pragma once
#include <string>
#include <memory>
#include "../Base/SFEmitter.h"
#include "../Camera/SFCamera.h"

namespace Starfall {
    using namespace std;
    class SFParser
    {
    public:
        static shared_ptr<SFSystem> parsePath(string path,int screenWidth,int screenHeight,shared_ptr<SFCamera> *cameraout,function<shared_ptr<GLTexture>(string path)> textureReadFunc);
    };
}



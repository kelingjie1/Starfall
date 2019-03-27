//
//  ViewController.m
//  StarfallExample
//
//  Created by lingtonke on 2019/3/19.
//  Copyright © 2019 Starfall. All rights reserved.
//

#import "ViewController.h"
#import <GLKit/GLKit.h>
#import "ObjectiveGL.h"
#import "Starfall.h"
using namespace ObjectiveGL;
using namespace Starfall;

@interface ViewController ()
{
    shared_ptr<GLContext> context;
    shared_ptr<GLFrameBuffer> framebuffer;
    shared_ptr<SFParticleSystem> particleSystem;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    context = GLContext::create();
    context->setCurrent();
    EAGLContext *eaglcontext = (__bridge EAGLContext*)context->context;
    ((GLKView*)self.view).context = eaglcontext;
    ((GLKView*)self.view).drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    SFParticleConfig config;
    config.maxParticleCount = 10000;
    particleSystem = make_shared<SFParticleSystem>();
    particleSystem->setup(config);
    NSString *respath = [NSBundle.mainBundle.resourcePath stringByAppendingPathComponent:@"Resource"];
    NSString *path = [respath stringByAppendingPathComponent:@"star.png"];
    
    auto texture = GLPlatform::createTextureFromFile([path cStringUsingEncoding:NSUTF8StringEncoding]);
    
    particleSystem->addParticle("", texture);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    context->setCurrent();
    if (!framebuffer) {
        int fb;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
        framebuffer = GLFrameBuffer::create(fb);
    }
    framebuffer->clear(1, 0, 0, 1);
    particleSystem->update(0);
    particleSystem->render(framebuffer);
    
}


@end

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
#import <glm/glm.hpp>
#import <glm/ext/matrix_transform.hpp>
#import <glm/ext/matrix_clip_space.hpp>
#import <glm/ext/scalar_constants.hpp>
#import <glm/gtc/type_ptr.hpp>

using namespace ObjectiveGL;
using namespace Starfall;

@interface ViewController ()
{
    shared_ptr<GLContext> context;
    shared_ptr<GLFrameBuffer> framebuffer;
    shared_ptr<SFParticleSystem> particleSystem;
    NSDate *date;
}
@property (nonnull) UILabel *label;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.label = [[UILabel alloc] initWithFrame:CGRectMake(100, 100, 1000, 100)];
    [self.view addSubview:self.label];
    date = [NSDate date];
    context = GLContext::create();
    context->setCurrent();
    EAGLContext *eaglcontext = (__bridge EAGLContext*)context->context;
    ((GLKView*)self.view).context = eaglcontext;
    ((GLKView*)self.view).drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    self.preferredFramesPerSecond = 60;
    SFParticleConfig config;
    config.maxParticleCount = 100000;
//    config.usePointRenderer = true;
//    config.pointRenderScale = 4.0;
    //config.useDefferredRendering = true;
    config.screenSize = make_pair(self.view.bounds.size.width*UIScreen.mainScreen.scale, self.view.bounds.size.height*UIScreen.mainScreen.scale);
    particleSystem = make_shared<SFParticleSystem>();
    particleSystem->setup(config);
    NSString *respath = [NSBundle.mainBundle.resourcePath stringByAppendingPathComponent:@"Resource"];
    NSString *path = [respath stringByAppendingPathComponent:@"star.png"];
    NSString *compute = [respath stringByAppendingPathComponent:@"star.compute"];
    NSString *str = [NSString stringWithContentsOfFile:compute encoding:NSUTF8StringEncoding error:nil];
    auto texture = GLPlatform::createTextureFromFile([path cStringUsingEncoding:NSUTF8StringEncoding]);
    
    particleSystem->addParticle([str cStringUsingEncoding:NSUTF8StringEncoding], texture);
    
    auto projection = glm::perspective(glm::pi<float>()*0.25f, (float)(self.view.bounds.size.width/self.view.bounds.size.height), 0.1f, 1000.f);
    auto view = glm::lookAt(glm::vec3(180,200,0), glm::vec3(0,0,0), glm::vec3(0,1,0));
    auto vp = projection*view;
    auto ptr = glm::value_ptr(vp);
    auto point = glm::vec4(0,0,0,1);
    auto result = vp*point;
    vector<float> matrix(ptr,ptr+16);
    particleSystem->setTransformMatrix(matrix);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    context->setCurrent();
    if (!framebuffer) {
        int fb;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
        framebuffer = GLFrameBuffer::create(fb);
    }
    framebuffer->clear(1, 0, 0, 1);
    NSDate *t0 = [NSDate date];
    particleSystem->update(self.timeSinceLastDraw);
    NSDate *t1 = [NSDate date];
    NSLog(@"update:%f",[t1 timeIntervalSinceDate:t0]);
    particleSystem->render(framebuffer);
    NSDate *t2 = [NSDate date];
    NSLog(@"render:%f",[t2 timeIntervalSinceDate:t1]);
    NSDate *newDate = [NSDate date];
    self.label.text = [NSString stringWithFormat:@"%f",[newDate timeIntervalSinceDate:date]];
    date = newDate;
}


@end

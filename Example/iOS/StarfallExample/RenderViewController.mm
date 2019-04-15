//
//  ViewController.m
//  StarfallExample
//
//  Created by lingtonke on 2019/3/19.
//  Copyright © 2019 Starfall. All rights reserved.
//

#import "RenderViewController.h"
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

@interface RenderViewController ()
{
    shared_ptr<GLContext> context;
    shared_ptr<GLFrameBuffer> framebuffer;
    shared_ptr<SFParticleSystem> particleSystem;
    NSDate *date;
}
@property (nonnull) UILabel *label;
@end

@implementation RenderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UITapGestureRecognizer *gr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap)];
    [self.view addGestureRecognizer:gr];
    
    self.label = [[UILabel alloc] initWithFrame:CGRectMake(100, 100, 1000, 100)];
    [self.view addSubview:self.label];
    date = [NSDate date];
    context = GLContext::create();
    context->setCurrent();
    EAGLContext *eaglcontext = (__bridge EAGLContext*)context->context;
    ((GLKView*)self.view).context = eaglcontext;
    ((GLKView*)self.view).drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    self.preferredFramesPerSecond = 60;
    
    NSString *respath = [NSBundle.mainBundle.resourcePath stringByAppendingPathComponent:@"Resource"];
    SFParticleConfig config;
    NSDictionary *dic = [self configDic:[respath stringByAppendingPathComponent:self.name]];
    config.maxParticleCount = [dic[@"maxParticleCount"] intValue];
    config.usePointRenderer = [dic[@"usePointRenderer"] boolValue];
    config.pointRenderScale = [dic[@"pointRenderScale"] floatValue];
    config.useDefferredRendering = [dic[@"useDefferredRendering"] boolValue];
    config.screenSize = make_pair(self.view.bounds.size.width*UIScreen.mainScreen.scale, self.view.bounds.size.height*UIScreen.mainScreen.scale);
    particleSystem = make_shared<SFParticleSystem>();
    particleSystem->setup(config);
    
    NSArray *particles = [dic[@"particles"] componentsSeparatedByString:@"|"];
    NSArray *textures = [dic[@"textures"] componentsSeparatedByString:@"|"];
    NSAssert(particles.count==textures.count, @"particles != textures");
    for (int i=0; i<particles.count; i++) {
        NSString *path = [respath stringByAppendingPathComponent:textures[i]];
        NSString *compute = [respath stringByAppendingPathComponent:particles[i]];
        NSString *str = [NSString stringWithContentsOfFile:compute encoding:NSUTF8StringEncoding error:nil];
        auto texture = GLPlatform::createTextureFromFile([path cStringUsingEncoding:NSUTF8StringEncoding]);
        particleSystem->addParticle([str cStringUsingEncoding:NSUTF8StringEncoding], texture);
    }
    float eyeX = [dic[@"eyeX"] floatValue];
    float eyeY = [dic[@"eyeY"] floatValue];
    float eyeZ = [dic[@"eyeZ"] floatValue];
    
    float lookAtX = [dic[@"lookAtX"] floatValue];
    float lookAtY = [dic[@"lookAtY"] floatValue];
    float lookAtZ = [dic[@"lookAtZ"] floatValue];
    
    auto projection = glm::perspective(glm::pi<float>()*0.25f, (float)(self.view.bounds.size.width/self.view.bounds.size.height), 0.1f, 1000.f);
    auto view = glm::lookAt(glm::vec3(eyeX,eyeY,eyeZ), glm::vec3(lookAtX,lookAtY,lookAtZ), glm::vec3(0,1,0));
    auto vp = projection*view;
    auto ptr = glm::value_ptr(vp);
    auto point = glm::vec4(0,0,0,1);
    auto result = vp*point;
    vector<float> matrix(ptr,ptr+16);
    particleSystem->setTransformMatrix(matrix);
}

- (void)tap {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (NSDictionary*)configDic:(NSString*)path {
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    NSString *fullstr = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    NSArray *list = [fullstr componentsSeparatedByString:@"\n"];
    for (NSString *str in list) {
        NSArray *params = [str componentsSeparatedByString:@":"];
        if (params.count == 2) {
            dic[params[0]] = params[1];
        }
    }
    return dic;
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

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
@property (nonatomic) UILabel *label;
@property (nonatomic) float scale;
@property (nonatomic) float scaleOffset;
@property (nonatomic) float rx;
@property (nonatomic) float ry;
@property (nonatomic) float rxoffset;
@property (nonatomic) float ryoffset;

@end

@implementation RenderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.scale = 200;
    self.scaleOffset = 1;
    
    UITapGestureRecognizer *gr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap)];
    [self.view addGestureRecognizer:gr];
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
    [self.view addGestureRecognizer:pan];
    UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
    [self.view addGestureRecognizer:pinch];
    
    self.label = [[UILabel alloc] initWithFrame:CGRectMake(100, 100, 1000, 100)];
    [self.view addSubview:self.label];
    date = [NSDate date];
    context = GLContext::create();
    context->setCurrent();
    EAGLContext *eaglcontext = [EAGLContext currentContext];
    ((GLKView*)self.view).context = eaglcontext;
    ((GLKView*)self.view).drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    self.preferredFramesPerSecond = 60;
    
    NSString *respath = [NSBundle.mainBundle.resourcePath stringByAppendingPathComponent:@"Resource"];

    NSString *path = [respath stringByAppendingPathComponent:self.name];
    NSDictionary *dic = [self configDic:path];
    
    particleSystem = SFParticleParser::parsePath([path cStringUsingEncoding:NSUTF8StringEncoding], self.view.bounds.size.width*UIScreen.mainScreen.scale, self.view.bounds.size.height*UIScreen.mainScreen.scale);
    
    
    self.rx = [dic[@"rotationX"] floatValue];
    self.ry = [dic[@"rotationY"] floatValue];
    self.scale = [dic[@"scale"] floatValue];
    
    [self updateMatrix];
}

- (void)updateMatrix {
    glm::vec4 v(0,0,1,0);
    glm::mat4 m(1);
    m = glm::rotate(m, self.rx+self.rxoffset, glm::vec3(0,1,0));
    m = glm::rotate(m, self.ry+self.ryoffset, glm::vec3(1,0,0));
    v = m*v;
    v *= self.scale*self.scaleOffset;
    auto projection = glm::perspective(glm::pi<float>()*0.25f, (float)(self.view.bounds.size.width/self.view.bounds.size.height), 0.1f, 1000.f);
    glm::vec3 eye(v.x,v.y,v.z);
    auto view = glm::lookAt(eye, glm::vec3(0,0,0), glm::vec3(0,1,0));
    auto vp = projection*view;
    auto ptr = glm::value_ptr(vp);
    auto point = glm::vec4(0,0,0,1);
    auto result = vp*point;
    vector<float> matrix(ptr,ptr+16);
    particleSystem->setTransformMatrix(matrix);
    NSLog(@"eye:%f,%f,%f",v.x,v.y,v.z);
}

- (void)pan:(UIPanGestureRecognizer*)gr {
    CGPoint point = [gr translationInView:self.view];
    self.rxoffset = -point.x*0.01;
    self.ryoffset = -point.y*0.01;
    if (gr.state == UIGestureRecognizerStateEnded) {
        self.rx += self.rxoffset;
        self.ry += self.ryoffset;
        self.rxoffset = 0;
        self.ryoffset = 0;
    }
    [self updateMatrix];
    
}

- (void)pinch:(UIPinchGestureRecognizer*)gr {
    self.scaleOffset = 1/gr.scale;
    if (gr.state == UIGestureRecognizerStateEnded) {
        self.scale *= self.scaleOffset;
        self.scaleOffset = 1;
    }
    [self updateMatrix];
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

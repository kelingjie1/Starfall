//
//  ViewController.m
//  StarfallExample
//
//  Created by lingtonke on 2019/3/19.
//  Copyright © 2019 Starfall. All rights reserved.
//

#import "RenderViewController.h"
#import <GLKit/GLKit.h>
#import <ObjectiveGL/ObjectiveGL.h>
#import "Starfall.h"
#import <glm/glm.hpp>
#import <glm/ext/matrix_transform.hpp>
#import <glm/ext/matrix_clip_space.hpp>
#import <glm/ext/scalar_constants.hpp>
#import <glm/gtc/type_ptr.hpp>
#import <ObjectiveGL/Platform/ios/GLIOSUtil.h>
#import <ObjectiveGL/Platform/ios/GLIOSContext.h>


using namespace ObjectiveGL;
using namespace Starfall;

@interface RenderViewController ()
{
    shared_ptr<GLContext> context;
    shared_ptr<GLFrameBuffer> framebuffer;
    shared_ptr<SFSystem> particleSystem;
    shared_ptr<SFCamera> camera;
    int frames;
    float time;
    float avgFPS;
    NSDate *date;
}
@property (nonatomic) UILabel *label;
@property (nonatomic) float distance;
@property (nonatomic) float distanceOffset;
@property (nonatomic) float rx;
@property (nonatomic) float ry;
@property (nonatomic) float rxoffset;
@property (nonatomic) float ryoffset;

@end

@implementation RenderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.distance = 200;
    self.distanceOffset = 1;
    
    UITapGestureRecognizer *gr = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tap)];
    [self.view addGestureRecognizer:gr];
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
    [self.view addGestureRecognizer:pan];
    UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
    [self.view addGestureRecognizer:pinch];
    
    self.label = [[UILabel alloc] initWithFrame:CGRectMake(10, 100, 1000, 100)];
    self.label.numberOfLines = 0;
    self.label.textColor = [UIColor blueColor];
    [self.view addSubview:self.label];
    date = [NSDate date];
    context = GLIOSContext::create();
    context->setCurrent();
    EAGLContext *eaglcontext = [EAGLContext currentContext];
    ((GLKView*)self.view).context = eaglcontext;
    ((GLKView*)self.view).drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    self.preferredFramesPerSecond = 60;
    
    NSString *respath = [NSBundle.mainBundle.resourcePath stringByAppendingPathComponent:@"Resource"];

    NSString *path = [respath stringByAppendingPathComponent:self.name];
    
    particleSystem = SFParser::parsePath([path cStringUsingEncoding:NSUTF8StringEncoding],
                                         self.view.bounds.size.width*UIScreen.mainScreen.scale,
                                         self.view.bounds.size.height*UIScreen.mainScreen.scale,
                                         &camera,
                                         [=](string path)->shared_ptr<GLTexture> {
                                             return GLIOSUtil::createTextureFromFile(path);
                                         });
    self.rx = camera->getRotationX();
    self.ry = camera->getRotationY();
    self.distance = camera->getDistance();
}

- (void)updateMatrix {
    camera->setPositionOrbit(self.rx+self.rxoffset, self.ry+self.ryoffset, self.distance*self.distanceOffset);
    particleSystem->setTransformMatrix(camera->getVPMatrix());
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
    self.distanceOffset = 1/gr.scale;
    if (gr.state == UIGestureRecognizerStateEnded) {
        self.distance *= self.distanceOffset;
        self.distanceOffset = 1;
    }
    [self updateMatrix];
}

- (void)tap {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    context->setCurrent();
    if (!framebuffer) {
        int fb;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
        framebuffer = GLFrameBuffer::create(fb);
    }
    framebuffer->clear(0, 0, 0, 1);
    particleSystem->update(self.timeSinceLastDraw);
    particleSystem->render(framebuffer);
    auto monitor = particleSystem->getMonitor();
    
    frames++;
    time+=self.timeSinceLastDraw;
    if (time>3) {
        avgFPS = frames/time;
        frames = 0;
        time = 0;
    }
    
    self.label.text = [NSString stringWithFormat:@"particleCount:\n%d\nupdateCost:\n%f\nrenderCost:\n%f\nframeDuration:\n%f\nFPS:%.2f\navgFPS:%.2f\n",monitor->particleCount,monitor->updateCost,monitor->renderCost,self.timeSinceLastDraw,1/self.timeSinceLastDraw,avgFPS];
    [self.label sizeToFit];
}


@end

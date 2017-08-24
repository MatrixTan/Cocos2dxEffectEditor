//
//  ARViewController.m
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/4.
//

#import <Foundation/Foundation.h>
#import "ARViewController.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

//#import "ARManager.hpp"

@interface ARViewController () <ARSCNViewDelegate>

@property (nonatomic, strong) IBOutlet ARSCNView *sceneView;
@property CVEAGLContext *_context;
@property CVOpenGLESTextureCacheRef _textureCache;

@end


@implementation ARViewController

+ (instancetype) createView
{
    ARViewController *controller = [[self alloc] init];
    controller.sceneView = [[ARSCNView alloc] initWithFrame:CGRectMake(0, 0, 1, 1)];
    //[self.view addSubview:self.sceneView];
    controller.view = controller.sceneView;
    
    controller.sceneView.showsStatistics = YES;
    
    // Create a new scene
    SCNScene *scene = [SCNScene sceneNamed:@"arres.scnassets/yu_model.DAE"];
    
    SCNAnimation *animation = [SCNAnimation animationNamed:@"arres.scnassets/yu_animation.DAE"];
    
    //scene.rootNode
    SCNNode *yu = [scene.rootNode childNodeWithName:@"Box002" recursively:true];
    //scene.rootNode.scale = SCNVector3Make(0.01, 0.01, 0.01);
    [yu addAnimation:animation forKey:@"yu_a"];
    // Set the scene to the view
    controller.sceneView.scene = scene;
    
    // Set the view's delegate
    controller.sceneView.delegate = controller;
    
    return controller;
}

- (void*)update:(CVEAGLContext) glContext{
    ARFrame *frame = self.sceneView.session.currentFrame;
    CVPixelBufferRef image = frame.capturedImage;
    if(!image){
        return 0;
    }
    
    return image;
}

- (ARSession*)getSession{
    return self.sceneView.session;
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)update {
    
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    // Create a session configuration
    ARWorldTrackingSessionConfiguration *configuration = [ARWorldTrackingSessionConfiguration new];
    
    // Run the view's session
    [self.sceneView.session runWithConfiguration:configuration];
    
    //[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(update) userInfo:nil repeats:true];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    
    // Pause the view's session
    [self.sceneView.session pause];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - ARSCNViewDelegate

/*
 // Override to create and configure nodes for anchors added to the view's session.
 - (SCNNode *)renderer:(id<SCNSceneRenderer>)renderer nodeForAnchor:(ARAnchor *)anchor {
 SCNNode *node = [SCNNode new];
 
 // Add geometry to the node...
 
 return node;
 }
 */

- (void)session:(ARSession *)session didFailWithError:(NSError *)error {
    // Present an error message to the user
    
}

- (void)sessionWasInterrupted:(ARSession *)session {
    // Inform the user that the session has been interrupted, for example, by presenting an overlay
    
}

- (void)sessionInterruptionEnded:(ARSession *)session {
    // Reset tracking and/or remove existing anchors if consistent tracking is required
    
}

@end

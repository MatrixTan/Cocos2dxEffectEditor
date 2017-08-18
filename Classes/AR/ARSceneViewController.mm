//
//  ARSceneViewController.m
//  EffectEditor
//
//  Created by Matrix on 2017/8/16.
//
//

#import <Foundation/Foundation.h>
#import "ARSceneViewController.h"
#import <ARKit/ARKit.h>
#include "ARManager.hpp"

@interface ARSceneViewController () <ARSCNViewDelegate>

@property (nonatomic, strong) IBOutlet ARSCNView *sceneView;

@end


@implementation ARSceneViewController

+ (instancetype) createView
{
    ARSceneViewController *controller = [[self alloc] init];
    return controller;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.sceneView = [[ARSCNView alloc] initWithFrame:self.view.bounds];
    self.sceneView.showsStatistics = YES;
    [self.view addSubview:self.sceneView];
    
    // Set the view's delegate
    self.sceneView.delegate = self;
}

- (void)stopAR{
    ee::ARManager::getInstance()->stopSceneView();
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    // Create a session configuration
    ARWorldTrackingSessionConfiguration *configuration = [ARWorldTrackingSessionConfiguration new];
    
    // Run the view's session
    [self.sceneView.session runWithConfiguration:configuration];
    
    //[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(update) userInfo:nil repeats:true];
    
    // Create a new scene
    SCNScene *scene = [SCNScene sceneNamed:@"arres.scnassets/arres.scn"];
    // Set the scene to the view
    self.sceneView.scene = scene;
    SCNNode *node = [scene.rootNode childNodeWithName:@"tree" recursively:true];
    [node setScale:(SCNVector3Make(0.01, 0.01, 0.01))];
    node.worldPosition = SCNVector3Make(0.0, -0.5, -0.5);
    
    SCNNode *testNode = [scene.rootNode childNodeWithName:@"test_box" recursively:true];
    testNode.scale = SCNVector3Make(0.01, 0.01, 0.01);
    
    SCNScene *monsterScene = [SCNScene sceneNamed:@"arres.scnassets/test_sphere_monster.DAE"];
    SCNNode *monsterNode = [monsterScene.rootNode childNodeWithName:@"Sphere001" recursively:true];
    SCNNode *replaceNode = [scene.rootNode childNodeWithName:@"Sphere001" recursively:true];
    [replaceNode.parentNode addChildNode:monsterNode];
    monsterNode.position = replaceNode.position;
    monsterNode.scale = replaceNode.scale;
    monsterNode.rotation = replaceNode.rotation;
    replaceNode.hidden = true;
    
    UIView *ui = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.bounds.size.width, 100)];
    [ui setAlpha:0.5];
    ui.backgroundColor = [UIColor colorWithWhite:1.0 alpha:0.5];
    [self.view addSubview:ui];
    
    
    
    UIButton *closeButton = [UIButton buttonWithType:UIButtonTypeCustom];
    closeButton.frame = CGRectMake(10, 10, 80, 40);
    closeButton.titleLabel.text = @"close";
    [closeButton setBackgroundImage:[UIImage imageNamed:@"res/qrcode.jpg"] forState:UIControlStateNormal];
    [closeButton addTarget:self action:@selector(stopAR) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:closeButton];
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

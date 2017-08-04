//
//  ARViewController.m
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/4.
//

#import <Foundation/Foundation.h>
#import "ARViewController.h"

@interface ARViewController () <ARSCNViewDelegate>

@property (nonatomic, strong) IBOutlet ARSCNView *sceneView;

@end


@implementation ARViewController

+ (instancetype) createView
{
    return [[self alloc] init];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIView *test = [[UIView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:test];
    test.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.5];
    
    self.sceneView = [[ARSCNView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:self.sceneView];
    // Set the view's delegate
    self.sceneView.delegate = self;
    
    // Show statistics such as fps and timing information
    self.sceneView.showsStatistics = YES;
    
    // Create a new scene
    SCNScene *scene = [SCNScene sceneNamed:@"arres.scnassets/arres.scn"];
    
    // Set the scene to the view
    self.sceneView.scene = scene;
 
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    // Create a session configuration
    ARWorldTrackingSessionConfiguration *configuration = [ARWorldTrackingSessionConfiguration new];
    
    // Run the view's session
    [self.sceneView.session runWithConfiguration:configuration];
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

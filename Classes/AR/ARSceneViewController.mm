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
#import "ARSceneUIView.h"

@interface ARSceneViewController () <ARSCNViewDelegate, CAAnimationDelegate>

@property (nonatomic, strong) IBOutlet ARSCNView *sceneView;
@property (nonatomic, strong) IBOutlet ARSceneUIView *uiView;
@property (nonatomic, strong) IBOutlet UIButton *uiButton;
@property (nonatomic) int currentProcess;
@property (nonatomic) int maxProcess;
@property (nonatomic, strong) IBOutlet UIImageView *uiButtonImageMonster;
@property (nonatomic, strong) IBOutlet UILabel *uiLabelMonsterNumber;
@property (nonatomic, strong) IBOutlet UILabel *uiLabelGetReward;
@property (strong) NSString *uiRewardString;

@property (nonatomic, strong) IBOutlet UIImageView *uiFlyMonster;
@property (nonatomic, strong) IBOutlet NSMutableArray *monsterAnimationSequence;
@end


@implementation ARSceneViewController

+ (instancetype) createView
{
    ARSceneViewController *controller = [[self alloc] init];
    return controller;
}

- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag
{
    [self nextUIMonsterAnimation];
}

- (void)nextUIMonsterAnimation{
    if(self.monsterAnimationSequence.count > 0){
        CABasicAnimation *animation = [self.monsterAnimationSequence objectAtIndex:0];
        [self.uiFlyMonster.layer addAnimation:animation forKey:@"an"];
        [self.monsterAnimationSequence removeObjectAtIndex:0];
    }else{
        [self.uiFlyMonster removeFromSuperview];
        self.uiFlyMonster = nil;
        self.currentProcess++;
        [self updateProcess];
    }
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

- (void)setGetRewardString:(NSString *)rewardString{
    self.uiRewardString = rewardString;
}

- (void)setCurrentProcess:(int)process maxNumber:(int)maxProcess{
    self.currentProcess = process;
    self.maxProcess = maxProcess;
}

- (void)updateProcess{
    self.uiLabelMonsterNumber.text = [NSString stringWithFormat:@"%d/%d",self.currentProcess,self.maxProcess];
    if(self.currentProcess >= self.maxProcess){
        self.uiLabelGetReward.hidden = false;
        self.uiLabelMonsterNumber.hidden = true;
        self.uiButtonImageMonster.hidden = true;
    }else{
        self.uiLabelGetReward.hidden = true;
        self.uiLabelMonsterNumber.hidden = false;
        self.uiButtonImageMonster.hidden = false;
    }
}

- (void)onTap:(UITapGestureRecognizer*) tgr{
    CGPoint pt = [tgr locationInView:self.sceneView];
    auto results = [self.sceneView hitTest:pt options:nil];
    if(results.count > 0){
        for(SCNHitTestResult *oneResult in results){
            //oneResult.node;
            auto nodeName = oneResult.node.name;
            if([nodeName isEqualToString:@"Sphere004"] || [nodeName isEqualToString:@"Sphere001"]){
                oneResult.node.hidden = true;
                NSString *thumbnailFile = [NSString stringWithFormat:@"%@/%@.png", [[NSBundle mainBundle] resourcePath], @"arres.scnassets/images/0_item_41.png"];
                UIImage *image = [[UIImage alloc] initWithContentsOfFile:thumbnailFile];
                self.uiFlyMonster = [[UIImageView alloc] initWithImage:image];
                [self.uiView addSubview:self.uiFlyMonster];
                self.uiFlyMonster.layer.position = pt;
                SCNParticleSystem* particle_ui = [SCNParticleSystem particleSystemNamed:@"star_tail.sks" inDirectory:nil];
                
                CABasicAnimation *scaleAnimation = [CABasicAnimation animationWithKeyPath:@"transform.scale"];
                scaleAnimation.fromValue = [NSNumber numberWithFloat:0.3];
                scaleAnimation.toValue = [NSNumber numberWithFloat:1.0];
                scaleAnimation.duration = 0.5;
                scaleAnimation.repeatCount = 0;
                scaleAnimation.removedOnCompletion = NO;
                scaleAnimation.fillMode = kCAFillModeForwards;
                scaleAnimation.delegate = self;
                [scaleAnimation setValue:@"1" forKey:@"animation_name"];
                
                CABasicAnimation *scaleAnimation2 = [CABasicAnimation animationWithKeyPath:@"transform.scale"];
                scaleAnimation2.fromValue = [NSNumber numberWithFloat:1.0];
                scaleAnimation2.toValue = [NSNumber numberWithFloat:0.5];
                scaleAnimation2.duration = 1.0;
                scaleAnimation2.repeatCount = 0;
                scaleAnimation2.removedOnCompletion = NO;
                scaleAnimation2.fillMode = kCAFillModeForwards;
                scaleAnimation2.delegate = self;
                [scaleAnimation2 setValue:@"2" forKey:@"animation_name"];
                
                
                CABasicAnimation *moveAnimation = [CABasicAnimation animationWithKeyPath:@"position"];
                moveAnimation.fromValue = [NSValue valueWithCGPoint:pt];
                float moveToX = self.uiButtonImageMonster.layer.position.x + self.uiButton.frame.origin.x;
                float moveToY = self.uiButtonImageMonster.layer.position.y + self.uiButton.frame.origin.y;
                moveAnimation.toValue = [NSValue valueWithCGPoint:CGPointMake(moveToX, moveToY)];
                moveAnimation.duration = 1.0;
                moveAnimation.repeatCount = 0;
                moveAnimation.removedOnCompletion = NO;
                moveAnimation.fillMode = kCAFillModeForwards;
                moveAnimation.delegate = self;
                [moveAnimation setValue:@"3" forKey:@"animation_name"];
                
                
                CAAnimationGroup *moveAnimationGroup = [CAAnimationGroup animation];
                moveAnimationGroup.animations = @[scaleAnimation2, moveAnimation];
                moveAnimationGroup.duration = 1.0f;
                moveAnimationGroup.delegate = self;
                moveAnimationGroup.fillMode = kCAFillModeForwards;
                moveAnimationGroup.removedOnCompletion = NO;
                [moveAnimationGroup setValue:@"4" forKey:@"aniamtion_name"];
                
                self.monsterAnimationSequence = [NSMutableArray arrayWithArray:@[scaleAnimation, moveAnimationGroup]];
                [self nextUIMonsterAnimation];
            }
        }
    }
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
    testNode.hidden = true;
    
    SCNScene *monsterScene = [SCNScene sceneNamed:@"arres.scnassets/test_sphere_monster.DAE"];
    SCNNode *monsterNode = [monsterScene.rootNode childNodeWithName:@"Sphere001" recursively:true];
    SCNNode *replaceNode = [scene.rootNode childNodeWithName:@"Sphere001" recursively:true];
    [replaceNode.parentNode addChildNode:monsterNode];
    monsterNode.position = replaceNode.position;
    monsterNode.scale = replaceNode.scale;
    monsterNode.rotation = replaceNode.rotation;
    replaceNode.hidden = true;
    
    SCNParticleSystem* particle = [SCNParticleSystem particleSystemNamed:@"fly_effect.scnp" inDirectory:nil];
    [monsterNode addParticleSystem:particle];
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTap:)];
    tap.numberOfTapsRequired = 1;
    tap.numberOfTouchesRequired = 1;
    [self.sceneView addGestureRecognizer:tap];
    
    CGSize windowSize = self.view.bounds.size;
    
    self.uiView = [[ARSceneUIView alloc] initWithFrame:CGRectMake(0, 0, windowSize.width, windowSize.height)];
    self.uiView.backgroundColor = [UIColor colorWithWhite:1.0 alpha:0.0];
    [self.view addSubview:self.uiView];
    
    UIButton *closeButton = [UIButton buttonWithType:UIButtonTypeCustom];
    closeButton.frame = CGRectMake(10, 10, 80, 40);
    closeButton.titleLabel.text = @"close";
    [closeButton setBackgroundImage:[UIImage imageNamed:@"res/qrcode.jpg"] forState:UIControlStateNormal];
    [closeButton addTarget:self action:@selector(stopAR) forControlEvents:UIControlEventTouchUpInside];
    [self.uiView addSubview:closeButton];
    
    self.uiButton = [UIButton buttonWithType:UIButtonTypeCustom];
    self.uiButton.frame = CGRectMake(windowSize.width * 0.5 - 100, 100, 200, 80);
    [self.uiButton setBackgroundImage:[UIImage imageNamed:@"res/qrcode.jpg"] forState:UIControlStateNormal];
    [self.uiView addSubview:self.uiButton];
    
    NSString *monsterFile = [NSString stringWithFormat:@"%@/%@.png", [[NSBundle mainBundle] resourcePath], @"arres.scnassets/images/0_item_41.png"];
    UIImage *monsterImage = [[UIImage alloc] initWithContentsOfFile:monsterFile];
    self.uiButtonImageMonster = [[UIImageView alloc] initWithImage:monsterImage];
    [self.uiButton addSubview:self.uiButtonImageMonster];
    self.uiButtonImageMonster.frame = CGRectMake(0, 0, 74, 74);
    
    self.uiLabelMonsterNumber = [[UILabel alloc] init];
    [self.uiButton addSubview:self.uiLabelMonsterNumber];
    self.uiLabelMonsterNumber.frame = CGRectMake(90, 30, 100, 30);
    self.uiLabelMonsterNumber.font = [UIFont systemFontOfSize:40.0];
    [self updateProcess];
    
    self.uiLabelGetReward = [[UILabel alloc] init];
    [self.uiButton addSubview:self.uiLabelGetReward];
    self.uiLabelGetReward.frame = CGRectMake(20, 10, 160, 60);
    self.uiLabelGetReward.font = [UIFont systemFontOfSize:40];
    self.uiLabelGetReward.text = self.uiRewardString;
    self.uiLabelGetReward.textAlignment = NSTextAlignmentCenter;
    [self updateProcess];
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

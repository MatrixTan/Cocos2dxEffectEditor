//
//  ARSceneViewController.h
//  EffectEditor
//
//  Created by Matrix on 2017/8/16.
//
//

#ifndef ARSceneViewController_h
#define ARSceneViewController_h

#import <UIKit/UIKit.h>
#import <SceneKit/SceneKit.h>
#import <ARKit/ARKit.h>

@interface ARSceneViewController : UIViewController

+ (nonnull instancetype) createView;
- (void) setCurrentProcess:(int) process maxNumber:(int)maxProcess;
- (void) setGetRewardString:(nullable NSString*) rewardString;
@end


#endif /* ARSceneViewController_h */

//
//  ARViewController.h
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/4.
//

#ifndef ARViewController_h
#define ARViewController_h

#import <UIKit/UIKit.h>
#import <SceneKit/SceneKit.h>
#import <ARKit/ARKit.h>

@interface ARViewController : UIViewController

+ (nonnull instancetype) createView;
- (void*) update: (CVEAGLContext) glContext;
- (ARSession*) getSession;

@end

#endif /* ARViewController_h */

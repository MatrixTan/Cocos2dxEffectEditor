//
//  ARSceneUIViewController.m
//  EffectEditor
//
//  Created by Matrix on 2017/8/21.
//

#import <Foundation/Foundation.h>
#import "ARSceneUIView.h"

@interface ARSceneUIView()

@end


@implementation ARSceneUIView

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *result = [super hitTest:point withEvent:event];
    if(result == self){
        return nil;
    }
    return result;
}

@end

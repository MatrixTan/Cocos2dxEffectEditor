//
//  ARManager.cpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/4.
//

#include "ARManager.hpp"

#import "ARViewController.h"
#include "AppController.h"

void ARManager::startVRView()
{
    ARViewController* arview = [ARViewController createView];
    AppController *appController = (AppController*)[[UIApplication sharedApplication] delegate];
    [appController.viewController presentViewController:arview animated:YES completion:nil];
}

//
//  WIDAppDelegate.h
//  ShaderTest
//
//  Created by Home on 2012-12-27.
//  Copyright (c) 2012 Tobias Lensing. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface WIDAppDelegate : UIResponder <UIApplicationDelegate, ICUpdatable>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) ICScene* scene;
@property (strong, nonatomic) ICHostViewController *hostViewController;


@end

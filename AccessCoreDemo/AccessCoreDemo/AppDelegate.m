//
//  AppDelegate.m
//  AccessCoreDemo
//
//  Created by supertext on 15/6/5.
//  Copyright (c) 2015年 forgame. All rights reserved.
//

#import "AppDelegate.h"
#import <AccessCore/AccessCore.h>
#import "ADMainViewController.h"
@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  
    UIWindow *window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    
    window.backgroundColor=[UIColor whiteColor];
    
    self.window=window;
    
    ADMainViewController *mainController = [[ADMainViewController alloc] init];

    self.window.rootViewController=[[UINavigationController alloc] initWithRootViewController:mainController];
    
    [self.window makeKeyAndVisible];
    
    // 平台初始化方法
    
    [kACStandardChannel() application:application didFinishLaunchingWithOptions:launchOptions gamekey:@"test" delegate:mainController];
    
    return YES;
}

-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    // 处理支付宝回调,必须调用
    
    return [kACStandardChannel() application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
}

-(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    [kACStandardChannel() application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}

-(void)applicationDidEnterBackground:(UIApplication *)application
{
    // 游戏进入后台统计,需传入serverid
    
    [kACStandardChannel() statisticEnterBackground:@"1"];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [kACStandardChannel() suspendGame];

    // 游戏进入前台统计,需传入serverid
    [kACStandardChannel() statisticEnterForeground:@"1"];
}

@end

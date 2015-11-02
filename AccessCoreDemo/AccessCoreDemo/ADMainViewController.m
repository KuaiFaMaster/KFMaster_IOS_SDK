//
//  ADMainViewController.m
//  AccessCoreDemo
//
//  Created by supertext on 15/6/5.
//  Copyright (c) 2015年 forgame. All rights reserved.
//

#import "ADMainViewController.h"

@interface ADMainViewController ()

@property(nonatomic)BOOL isInited;

@end

@implementation ADMainViewController

-(void)viewDidLoad
{
    // 在游戏启动时监听初始化成功通知
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(sdkInitfinished) name:ACAccessChannelDidFinishInitNotification object:nil];
    
    // 登录通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(loginFinished:) name:ACAccessChannelDidFinishLoginNotification object:nil];
    
    // 注销成功通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(logoutFinished:) name:ACAccessChannelDidFinishLogoutNotification object:nil];
    
    // 购买通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(payResult:) name:ACAccessChannelDidFinishPaymentNotification object:nil];
    
    // 离开平台通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(leaveComplatform:) name:ACAccessChannelDidLeaveChannelPageNotification object:nil];

}

// 初始化成功回调
- (void)sdkInitfinished
{
    self.isInited = YES;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 10;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
    }
    if (indexPath.row == 0) {
        cell.textLabel.text = @"登陆";
    } else if (indexPath.row == 1) {
        cell.textLabel.text = @"充值";
    } else if (indexPath.row == 2) {
        cell.textLabel.text = @"用户中心";
    } else if (indexPath.row == 3) {
        cell.textLabel.text = @"注销";
    } else if (indexPath.row == 4) {
        cell.textLabel.text = @"点击按钮统计";
    }
    return cell;
}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    switch (indexPath.row) {
        case 0:
        {
            // 登录，一定要在初始化成功后调
            if (self.isInited) {
                [kACStandardChannel() showLoginView];
            } else {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"平台初始化失败" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
                [alert show];
            }
            
            break;
        }
        case 1:
        {
            // 支付
            
            ACOrderObject *order = [[ACOrderObject alloc] init];
            order.amount = @"1";
            order.productName = @"商品name";
            order.orderNumber = @"1111";
            order.productCount = @"10";
            order.productid = @"proID";
            order.orderNumber = [NSString stringWithFormat:@"%d",arc4random()%1000000];
            order.serverid = @"1";
            
            [kACStandardChannel() paymentWithOrder:order];
            break;
        }
        case 2:
        {
            // 显示个人中心
            
            if ([kACStandardChannel() isProvideUserCenter])
            {
                [kACStandardChannel() showUserCenter];
            }
            else
            {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"该渠道未提供用户中心接口" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
                [alert show];
            }
            break;
        }
        case 3:
        {
            // 注销
            if ([kACStandardChannel() isProvideLogout]) {
                if ([kACStandardChannel() isLogined] ) {
                    [kACStandardChannel() logout];
                } else {
                    NSLog(@"请先登录");
                }
            } else {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"该渠道未提供注销用户接口" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
                [alert show];
            }

            
            break;
        }
        case 4:
            
            // 点击按钮统计，需要传入点击按钮的名称，serverid
            
            [kACStandardChannel() statisticBtnClick:@"按钮名称" serverno:@"1"];
            break;
        default:
            break;
    }
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}


#pragma mark -----------------------------------------代理回调方法

/**
 *   登录代理回调
 *   userInfo：登录成功的用户信息
 *   error:    网络错误信息
 */
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishLoginWithUser:(ACUserObject *)userInfo error:(NSError *)error
{
    if (!error && userInfo.openid) {
        
        NSLog(@"登录成功,token = %@, openid = %@, 渠道唯一标识 ＝ %@",[kACStandardChannel() platformToken], userInfo.openid, [kACStandardChannel() channleName]);
        
        // 登录统计，需传入serverid
        [kACStandardChannel() statisticLogin:@"1"];
    } else {
        NSLog(@"登录失败");
    }
}

/**
 *   注销代理回调
 *   error:  网络错误信息
 */
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishLogoutWithError:(NSError *)error
{
    NSLog(@"注销成功");
    
}


/**
 *   购买商品代理回调
 *   orderid：订单号
 *   result:  购买结果
 *   error:   网络错误信息
 */
- (void)accessChannel:(id<ACAccessChannel>)channel didFinishPaymentWithOrderid:(NSString *)orderid result:(ACPaymentResult)result error:(NSError *)error
{
    if (!error) {
        if (result == ACPaymentResultSucceed) {
            NSLog(@"购买商品成功代理方法,orderid = %@", orderid);
        } else if (result == ACPaymentResultCancel) {
            NSLog(@"购买商品失败代理方法,-----用户取消购买");
        } else if (result == ACPaymentResultFailed) {
            NSLog(@"购买商品失败代理方法,-----购买失败");
        } else if (result == ACPaymentResultOrderInfoError) {
            NSLog(@"购买商品失败代理方法,-----订单填写有误");
        } else if (result == ACPaymentResultNologin) {
            NSLog(@"购买商品失败代理方法,-----未登录");
        } else if (result == ACPaymentResultBalanceNotEnough) {
            NSLog(@"购买商品失败代理方法,-----平台币余额不足");
        } else if (result == ACPaymentResultSameSerial) {
            NSLog(@"购买商品失败代理方法,-----订单号重复");
        } else if (result == ACPaymentResultOtherError) {
            NSLog(@"购买商品失败代理方法,-----其他错误信息");
        }
    } else {
        NSLog(@"网络错误----%@", error);
    }
}


#pragma mark -----------------------------------------通知回调方法


// 登录结果通知回调
- (void)loginFinished:(NSNotification *)notify
{
    ACUserObject *userObj = [notify.userInfo  valueForKey:kACNotificationValueKey];
    NSError *error = [notify.userInfo valueForKey:kACNotificationErrorKey];
    
    if (!error && userObj.openid) {
        
        NSLog(@"登录成功通知回调---openid = %@, token = %@, 渠道唯一标识 ＝ %@", userObj.openid, [kACStandardChannel()
                                                                                   platformToken], [kACStandardChannel() channleName]);
        // 登录统计，需传入serverid
        [kACStandardChannel() statisticLogin:@"1"];
    } else {
        NSLog(@"登录失败通知回调---error = %@", error);
    }
}

// 注销成功通知回调
- (void)logoutFinished:(NSNotification *)notify
{
    NSLog(@"---注销成功通知回调%@", notify.userInfo);
}

// 购买商品通知回调, 传回的userInfo为订单信息字典
- (void)payResult:(NSNotification *)notify
{
    NSString *orderid = [notify.userInfo valueForKey:kACNotificationValueKey];
    NSError *error = [notify.userInfo valueForKey:kACNotificationErrorKey];
    
    ACPaymentResult result = [[notify.userInfo valueForKey:kACNotificationStatusKey] integerValue];
    
    if (!error) {
        if (result == ACPaymentResultSucceed) {
            NSLog(@"购买商品成功通知回调,orderid = %@", orderid);
        } else if (result == ACPaymentResultCancel) {
            NSLog(@"购买商品失败通知回调,-----用户取消购买");
        } else if (result == ACPaymentResultFailed) {
            NSLog(@"购买商品失败通知回调,-----购买失败");
        } else if (result == ACPaymentResultOrderInfoError) {
            NSLog(@"购买商品失败通知回调,-----订单填写有误");
        } else if (result == ACPaymentResultNologin) {
            NSLog(@"购买商品失败通知回调,-----未登录");
        } else if (result == ACPaymentResultBalanceNotEnough) {
            NSLog(@"购买商品失败通知回调,-----平台币余额不足");
        } else if (result == ACPaymentResultSameSerial) {
            NSLog(@"购买商品失败通知回调,-----订单号重复");
        } else if (result == ACPaymentResultOtherError) {
            NSLog(@"购买商品失败通知回调,-----其他错误信息");
        }
    } else {
        NSLog(@"网络错误----%@", error);
    }
}

// 离开平台通知回调
- (void)leaveComplatform:(NSNotification *)notify
{
    ACChannelLeaveType type = [[notify.userInfo valueForKey:kACNotificationValueKey] integerValue];
    if (type == ACChannelLeaveTypeUserCenter) {
        NSLog(@"从个人中心离开");
    } else if (type == ACChannelLeaveTypePayment) {
        NSLog(@"从支付界面离开");
    } else if (type == ACChannelLeaveTypeLoginRegist) {
        NSLog(@"从登录、注册页离开");
    }
    
}


// 移除通知

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end

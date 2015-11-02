//
//  ACUserObject.h
//  AccessCore
//
//  Created by supertext on 15/5/7.
//  Email  supertext@icloud.com
//  Copyright (c) 2015年 forgame. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ACUserObject : NSObject//渠道用户基本信息对象
@property(nonatomic,copy,readonly)NSString *userid;             //渠道用户系统的唯一标识，可以理解为渠道的openid
@property(nonatomic,copy,readonly)NSString *username;           //名称 有可能是空的
@property(nonatomic,copy,readonly)NSString *nikcname;           //昵称 有可能是空的
@property(nonatomic,copy,readonly)NSString *openid;             //快发整合平台返回的openid
@property(nonatomic,copy,readonly)NSString *avatar;             //头像 有可能是空的
@property(nonatomic,copy,readonly)NSString *level;              //级别 有可能是空的
@property(nonatomic,copy,readonly)NSString *gender;             //性别 有可能是空的
@end

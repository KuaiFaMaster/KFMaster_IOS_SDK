//
//  ACOrderObject.h
//  AccessCore
//
//  Created by supertext on 15/5/7.
//  Email  supertext@icloud.com
//  Copyright (c) 2015年 forgame. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ACOrderObject : NSObject
@property (nonatomic,copy)NSString   *amount;             //支付金额,必填项
@property (nonatomic,copy)NSString   *productid;          //商品ID，91是必填参数，如果CP不填，我们随机填写
@property (nonatomic,copy)NSString   *productCount;       //商品数量，默认为1.
@property (nonatomic,copy)NSString   *orderNumber;        //外部订单号，由CP自由填写
@property (nonatomic,copy)NSString   *productName;        //商品名称,很多渠道都是必填参数，尽量填上。如果不填默认unkown
@property (nonatomic,copy)NSString   *notifyUrl;          //自定义回调地址,默认为channle.xml中的值,如果CP填了这个参数将覆盖channel.xml中的配置,如果未设置这个参数 将使用配置channel.xml中的配置
@property (nonatomic,copy)NSString   *paydesc;            //商户私有信息，CP自由填写
@property (nonatomic,copy)NSString   *roleid;             //角色ID，PP助手为必填参数，如果有尽量填上，如果不填我们随机填写
@property (nonatomic,copy)NSString   *serverid;           //游戏区服id，快用,xy为必填参数。如果有的话尽量填上，如果CP不填我们随机填写
@property (nonatomic,copy)NSString   *gameextend;         //游戏扩展参数，CP自由填写
@property (nonatomic,copy)NSString   *productDisplayTitle;//支付显示名称，CP自由填写

@property (nonatomic,copy,readonly)NSString   *orderid;   //统一平台生成的订单号
@end

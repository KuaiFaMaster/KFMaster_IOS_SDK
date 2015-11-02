//
//  ACAccessChannel.h
//  AccessCore
//
//  Created by supertext on 15/5/7.
//  Email  supertext@icloud.com
//  Copyright (c) 2015年 forgame. All rights reserved.
//
// @version 1.0


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#ifdef __cplusplus
#define ACKit_EXTERN		extern "C" __attribute__((visibility ("default")))
#else
#define ACKit_EXTERN	        extern __attribute__((visibility ("default")))
#endif

typedef NS_ENUM(NSInteger, ACPopoverPosiation) {
    ACPopoverPosiationDefualt,//渠道默认位置
    ACPopoverPosiationLeft,
    ACPopoverPosiationRight,
    ACPopoverPosiationTopLeft,
    ACPopoverPosiationTopRight,
    ACPopoverPosiationBottomLeft,
    ACPopoverPosiationBottomRight
};

typedef NS_ENUM(NSInteger, ACOrderStatus) {
    ACOrderStatusSucceed,//成功
    ACOrderStatusUntreated,//未处理
    ACOrderStatusPayfailed,//充值失败
    ACOrderStatusNotifyfailed//通知游戏方失败
};

typedef NS_ENUM(NSInteger, ACPaymentResult) {
    ACPaymentResultSucceed,//支付成功
    ACPaymentResultCancel,//用户取消支付
    ACPaymentResultOrderInfoError,//订单创建错误
    ACPaymentResultSameSerial,//订单号重复
    ACPaymentResultRefuse,//服务器拒绝
    ACPaymentResultFailed,//支付失败
    ACPaymentResultNetworkError,//忘了链接错误
    ACPaymentResultBalanceNotEnough,//余额不足
    ACPaymentResultNologin,//尚未登录
    ACPaymentResultOtherError//其他错误
};

@class      ACUserObject;
@class      ACOrderObject;
@class      UIApplication;
@protocol   ACAccessChannel;
@protocol   ACAccessChannelDelegate;



ACKit_EXTERN id<ACAccessChannel> kACStandardChannel();//获取标准化渠道单列对象

@protocol ACAccessChannel<NSObject>//渠道统一接入标准接口，直接调用对应渠道的对应方式实现，游戏方统一使用这一套接口就能完成所有渠道的接入

@optional

@property(nonatomic,strong,readonly)ACUserObject                *loginedUser;        //登录的用户信息
@property(nonatomic,weak  ,readonly)id<ACAccessChannelDelegate> delegate;            //事件回调代理 由application:didFinishLaunchingWithOptions:delegate:方法设置
@property(nonatomic,copy  ,readonly)NSString                    *appkey;             //渠道的appkey
@property(nonatomic,copy  ,readonly)NSString                    *appid;              //渠道的appid
@property(nonatomic,copy  ,readonly)NSString                    *sessionid;          //渠道返回的token 或者 sessionid
@property(nonatomic,copy  ,readonly)NSString                    *channleName;        //渠道标识
@property(nonatomic,copy  ,readonly)NSString                    *platformToken;      //快发整合平台返回的token，用于登录验证




#pragma mark ------------------------------------------------------------- initlize methods


/** 初始化方法
 *  @brief 应用初始化成功之后由游戏方调用此方法,此方法是快发整合平台的初始化方法
 *  @warning 此方法必须调用
 *  @note  just like this:
 
    - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
        //你的初始化代码
        [kACStandardChannel() application:application didFinishLaunchingWithOptions:launchOptions delegate:self];
        return YES;
    }
 
 *  @param application   UIApplication对象
 *  @param gamekey       在快发整合平台申请的gamekey
 *  @param launchOptions 加载选项
 *  @param delegate      委托对象 可以为空
 */
-(void)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
           gamekey:(NSString *)gamekey
          delegate:(id<ACAccessChannelDelegate>)delegate;

/**
 *  @brief 由CP在 appDelegate 的openURL 中调用
 
 *  @warning 此方法必须调用
 
 *  @note  just like this:
 *  -(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
    {
        return [kACStandardChannel() application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
    }
 
 *  @param application       UIApplication对象
 *  @param url               url将要处理的openurl
 *  @param sourceApplication 来源应用名称
 *  @param annotation        附加的信息
 *  @return 是否响应该openURL 事件
 */
-(BOOL)application:(UIApplication *)application
           openURL:(NSURL *)url
 sourceApplication:(NSString *)sourceApplication
        annotation:(id)annotation;
/**
 *  @brief 由CP在application:didRegisterForRemoteNotificationsWithDeviceToken:方法中调用
 *  @warning 必须接入此方法
 *  @param application UIApplication对象
 *  @param deviceToken 设备deviceToken
 */
-(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;


#pragma mark ---------------------------------------------------------login and register methods


/**登录注册相关方法 CP在适当的时机调用相关接口 完成渠道用户接入*/
-(void)showLoginView;                           //弹出登录视图
-(BOOL)isLogined;                               //判断用户是否登录
-(void)logout;                                  //注销
-(void)showUserCenter;                          //显示用户中心

-(void)setAutoLoginEnbled:(BOOL)isEnble;        //自动登录,默认不自动登录，如果调用这个方法将覆盖channel.xml里面的配置
-(void)switchAccount;                           //切换账号
-(void)hideView;                                //隐藏当前弹出的视图



#pragma mark ----------------------------------------------------------- payment methods
/** 支付相关方法 **/

/** @warning 如果是多区服游戏，订单必须传入serverid
 
 *  @brief 订单支付功能，如果order为空则弹出充值页面，如果该渠道不支持定额支付 则order将被忽略直接弹出充值页面
 *  @brief 如果用户没登录 回调中会收到ACPaymentResultNologin 的错误代码
 *  @param order 订单信息
 */
-(void)paymentWithOrder:(ACOrderObject *)order;

-(void)queryOrderWithid:(NSString *)orderid;//这里需要的订单号为快发整合平台生成的订单号，在支付结果回调后获得
-(void)queryOrderWithOrder:(ACOrderObject *)order;;


#pragma mark -----------------------------------------------------------  settings methods

/**设置相关方法*/
-(void)setPopoverHidden:(BOOL)hidden;           //显示或者隐藏浮窗，默认以channel.xml配置为准，如果该渠道没有浮窗功能则此方法无效。如果调用这个方法将覆盖channel.xml里面的配置

-(void)setPopoverPosiation:(ACPopoverPosiation)posiation;//设置浮窗位置，默认以channel.xml配置为准如果该渠道没有浮窗功能则此方法无效。如果调用这个方法将覆盖channel.xml里面的配置

-(void)checkUpdate;
//检查更新,如果没有检查更新，则此方法无效


#pragma mark ------------------------------------------------------------- game runtime methods


/*** 游戏运行相关方法，由游戏方在对应时机调用，如果对应渠道没有类似操作，下面3个方法为空实现**/
-(void)continueGame;                            //游戏由暂停到继续时调用
-(void)suspendGame;                             //游戏暂停时调用
-(void)stopGame;                                //结束游戏


#pragma mark -------------------------------------------------------------- 统计接口

/**
 *  @brief 统计用户登录, 建议在登录成功回调中调用.
 *  @param serverno 服务器编号
 */
-(void)statisticLogin:(NSString *)serverno;

/**
 *  @brief 统计区服登录
 *  @param serverno 服务器编号
 */
- (void)statisticServerLogin:(NSString *)serverno;

/**
 *  @brief 统计玩家升级
 *  @param serverno 服务器编号
 *  @param grade    玩家等级
 */
- (void)statisticUpgrade:(NSString *)serverno grade:(NSString *)grade;

/**
 *  @brief 统计游戏进入前台/上线， 在applicationWillEnterForeground:中调用
 *  @param serverno 服务器编号
 */
- (void)statisticEnterForeground:(NSString *)serverno;

/**
 *  @brief 统计游戏进入后台/下线，在applicationDidEnterBackground:中调用
 *  @param serverno 服务器编号
 */
-(void)statisticEnterBackground:(NSString *)serverno;

/**
 *  @brief 统计按钮点击
 *  @param name     点击按钮的名称
 *  @param serverno 服务器编号
 */
- (void)statisticBtnClick:(NSString *)name serverno:(NSString *)serverno;

/**
 *  @brief 创建角色统计
 *  @param serverno 服务器编号
 */
-(void)statisticCreateRole:(NSString *)serverno;

/**
 * @brief 崩溃记录, 在游戏启动时，初始化sdk之后调用
 */
- (void)statisticAbnormal;


#pragma mark ------------------------------------- 判断渠道是否提供接口
/**
 * 渠道是否提供用户中心
 */
- (BOOL)isProvideUserCenter;
/**
 * 渠道是否提供注销
 */
- (BOOL)isProvideLogout;
/**
 * 渠道是否提供暂停游戏
 */
- (BOOL)isProvideSuspendGame;
/**
 * 渠道是否提供继续游戏
 */
- (BOOL)isProvideContinueGame;
/**
 * 渠道是否提供切换账号
 */
- (BOOL)isProvideSwitchAccount;


#pragma mark ------------------------------------- 订单存储、获取、删除,轮询
- (void)saveOrderWithOrder:(ACOrderObject *)order;
- (NSArray *)getOrderArray;
- (void)removeOrder:(NSString *)orderid;

/**
 *  @brief 轮询查询订单状态
 *  @param timer     轮询时间间隔,单位:秒
 *  @param timeout 订单超时时间,单位:秒
 */
- (void)openCheckOrderLoopWithInterval:(NSTimeInterval)timer timeout:(NSTimeInterval)timeout;


#pragma mark ------------------------------------------------------------- NewGame手柄所有按钮设置

/**
 *  @brief    该API为新游手柄按键接入
 *  @warning  此方法需调用  需要按顺序往数组里面存入按键
 *  @param    sender为数组 里面按索引(0~15)放了手柄上对应的所有16（除了左摇杆、右摇杆）个按键 依次为:A B X Y leftShoulder(L1) rightShoulder(R1) leftTrigger(L2) rightTrigger(R2) buttonBack(返回键) buttonStart（start键） buttonLeftThumb（左摇杆按键） buttonRightThumb（右摇杆按键） dpadleft（十字键左键） dpadright（十字键右键） dpadup（十字键上键） dpaddown（十字键下键）   ||  每个按钮都有对应的通知回调
 */
- (void)setAllButton:(id)sender;


/** unity3d初始化方法
 *  @brief 应用初始化成功之后由游戏方调用此方法,此方法是快发整合平台的初始化方法
 *  @warning 此方法必须调用
 *  @param gamekey       在快发整合平台申请的gamekey
 *  @param delegate      委托对象 可以为空
 */
-(void)initWithGameKey:(NSString *)gamekey delegate:(id<ACAccessChannelDelegate>)delegate;


@end


ACKit_EXTERN NSString *const kACPaymentErrorDomain;//支付相关error对象的error domain
ACKit_EXTERN NSString *const kACRechangeErrorDomain;//充值相关error对象的error domain
ACKit_EXTERN NSString *const kACAuthErrorDomain;//登录注册相关error对象的error domain
ACKit_EXTERN NSInteger const kACCommonErrorCode;//通用错误码，多少情况的error对象的code为此值，个别情况会有特殊值

#pragma mark -----------------------------------------------------------------事件回调

@protocol ACAccessChannelDelegate <NSObject>//事件回调
@optional
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishLoginWithUser:(ACUserObject *)userInfo error:(NSError *)error;//登录完成后调用:如果成功userInfo不为空error为空，如果失败userInfo为空,error不为空.
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishLogoutWithError:(NSError *)error;//注销完成后调用,如果未能成功注销error 不为空
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishRegistWithError:(NSError *)error;//注册完成之后调用，如果注册失败error不为空

#pragma mark ---------------------------------支付相关，传入订单号代理回调接口----------------------

-(void)accessChannel:(id<ACAccessChannel>)channel didFinishPaymentWithOrderid:(NSString *)orderid
              result:(ACPaymentResult)result
               error:(NSError *)error;//支付结束之后调用，如果支付失败ACPaymentResult!=ACPaymentResultSucceed,如果渠道有错误信息将体现在error里面,这里返回的订单号是整合平台生成的订单号,如果渠道没有支付回调方法，那么这个回调不会调用，需要CP手动调用订单查询方法
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishRechangeWithOrderid:(NSString *)orderid
              result:(ACOrderStatus)result
               error:(NSError *)error;//充值结束后调用，有些渠道并没有,ACOrderStatus
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishQueryWithOrderid:(NSString *)orderid
              status:(ACOrderStatus)status
                desc:(NSString *)desc
               error:(NSError *)error;//订单查询完成之后回调，如果发生错误error不为空


#pragma mark ---------------------------------支付相关，传入完整订单代理回调接口----------------------

-(void)accessChannel:(id<ACAccessChannel>)channel didFinishPaymentWithOrder:(ACOrderObject *)order
              result:(ACPaymentResult)result error:(NSError *)error;
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishRechangeWithOrder:(ACOrderObject *)order result:(ACOrderStatus)result;
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishQueryWithOrder:(ACOrderObject *)order
              status:(ACOrderStatus)status
                desc:(NSString *)desc
               error:(NSError *)error;


//手柄相关回调
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishAllHandlerCallBack:(id)sender;    //针对所有手柄元素变化事件的回调
-(void)accessChannel:(id<ACAccessChannel>)channel didFinishSingleHandlerCallBack:(id)sender; //针对单个手柄元素注册事件回调
@end

typedef NS_ENUM(NSInteger, ACChannelLeaveType) {
    ACChannelLeaveTypeUnkown,/* 离开未知平台（预留状态）*/
    ACChannelLeaveTypeLoginRegist,/* 离开注册、登录页面 */
    ACChannelLeaveTypeUserCenter, /* 包括个人中心、游戏推荐、论坛 */
    ACChannelLeaveTypePayment/* 离开充值页（包括成功、失败）*/
};

ACKit_EXTERN NSString *const kACNotificationStatusKey;//BOOL isok = [userInfo[kACNotificationStatusKey] boolValue],标志通知回调对应事件状态 是bool值或者枚举值
ACKit_EXTERN NSString *const kACNotificationErrorKey;//NSErorr * error = [userInfo[kACNotificationErrorKey],通知回调的错误信息，isok==NO,这里保存失败信息
ACKit_EXTERN NSString *const kACNotificationValueKey;//id value = userInfo[kACNotificationValueKey],相应通知的回调值，无返回值则为空
ACKit_EXTERN NSString *const kACNotificationExtendKey;//id extend = userInfo[kACNotificationExtendKey],扩展信息字段 一般情况下为空
/* 系统通知:所有的通知发出的时间都是在 相应的事件的delegate调用之后*/
ACKit_EXTERN NSString *const ACAccessChannelDidFinishInitNotification;//userinfo = nil //sdk初始化之后回调
ACKit_EXTERN NSString *const ACAccessChannelDidFinishLoginNotification;//ACUserObject *user = userInfo[kACNotificationValueKey];//登录结束之后回调
ACKit_EXTERN NSString *const ACAccessChannelDidFinishLogoutNotification;//userinfo = nil //注销之后回调
ACKit_EXTERN NSString *const ACAccessChannelDidFinishRegistNotification;//注册结束之后回调
ACKit_EXTERN NSString *const ACAccessChannelDidOpenedPayPageNotification; //打开支付界面通知
ACKit_EXTERN NSString *const ACAccessChannelDidClosedPayPageNotification;//关闭支付界面通知
ACKit_EXTERN NSString *const ACAccessChannelDidFinishPaymentNotification;//ACPaymentResult resultCode = [userInfo[kACNotificationStatusKey] integerValue];NSString *orderid = userInfo[kACNotificationValueKey];//支付结束后的通知，只传订单号

ACKit_EXTERN NSString *const ACAccessChannelDidFinishOrderNotification;//ACPaymentResult resultCode = [userInfo[kACNotificationStatusKey] integerValue];NSString *orderid = userInfo[kACNotificationValueKey];//支付结束后的通知，传完整订单
ACKit_EXTERN NSString *const ACAccessChannelDidFinishRechangeNotification;//充值结束后的通知
ACKit_EXTERN NSString *const ACAccessChannelDidFinishQueryOrderNotification;//ACOrderStatus status = [userInfo[kACNotificationValueKey] integerValue];//查询订单结束之后的通知,只传订单号
ACKit_EXTERN NSString *const ACAccessChannelDidFinishCheckOrderNotification;//ACOrderStatus status = [userInfo[kACNotificationValueKey] integerValue];//查询订单结束之后的通知,传完整订单
ACKit_EXTERN NSString *const ACAccessChannelDidLeaveChannelPageNotification;//ACChannelLeaveType leaveType = [userinfo[kACNotificationValueKey] integerValue];//离开渠道页面之后的通知
ACKit_EXTERN NSString *const ACAccessChannelTempUserBecomeOfficialNotification;//userinfo = nil;//游客用户转正之后的通知

//手柄相关回调通知 cp可以在通知绑定的方法里面处理当前按键对应的触发事件
ACKit_EXTERN NSString *const ACAccessChannelButtonAPressNSNotification;//A
ACKit_EXTERN NSString *const ACAccessChannelButtonBPressNSNotification;//B
ACKit_EXTERN NSString *const ACAccessChannelButtonXPressNSNotification;//X
ACKit_EXTERN NSString *const ACAccessChannelButtonYPressNSNotification;//Y
ACKit_EXTERN NSString *const ACAccessChannelLeftShoulderPressNSNotification;//LeftShoulder
ACKit_EXTERN NSString *const ACAccessChannelRightShoulderPressNSNotification;//RightShoulder
ACKit_EXTERN NSString *const ACAccessChannelLeftTriggerPressNSNotification;//LeftTrigger
ACKit_EXTERN NSString *const ACAccessChannelRightTriggerPressNSNotification;//RightTrigger
ACKit_EXTERN NSString *const ACAccessChannelButtonBackPressNSNotification;//ButtonBack
ACKit_EXTERN NSString *const ACAccessChannelButtonStartPressNSNotification;//ButtonStart
ACKit_EXTERN NSString *const ACAccessChannelButtonLeftThumbPressNSNotification;//LeftThumb
ACKit_EXTERN NSString *const ACAccessChannelButtonRightThumbPressNSNotification;//RightThumb
ACKit_EXTERN NSString *const ACAccessChannelDpadUpPressedNSNotification;//DpadUp
ACKit_EXTERN NSString *const ACAccessChannelDpadDownPressedNSNotification;//DpadDown
ACKit_EXTERN NSString *const ACAccessChannelDpadLeftPressedNSNotification;//DpadLeft
ACKit_EXTERN NSString *const ACAccessChannelDpadRightPressedNSNotification;//DpadRight
ACKit_EXTERN NSString *const ACAccessChannelLeftThumbStickPressedNSNotification; //LeftThumbStick  左摇杆 userInfo里面存了左摇杆坐标变化 可以输出userInfo看下里面的值
ACKit_EXTERN NSString *const ACAccessChannelRightThumbStickPressedNSNotification;//RightThumbStick 右摇杆 userInfo里面存了右摇杆坐标变化 可以输出userInfo看下里面的值
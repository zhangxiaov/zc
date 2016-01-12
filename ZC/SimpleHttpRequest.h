//
//  SimpleHttpRequest.h
//  oc_demo
//
//  Created by zhangxinwei on 15/12/3.
//  Copyright © 2015年 zhangxinwei. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^ResponseBlock)(NSData* responseData);
typedef void (^ResponseErrorBlock)(NSError* error);
typedef void (^RequestExtentBlock)(NSOperation* operation);

@interface SimpleHttpRequest : NSObject

+ (void)postWithParams:(NSDictionary *)dict path:(NSString *)path compeletion:(ResponseBlock)compeletion;

+ (void)postWithParams:(NSDictionary *)dict path:(NSString *)path extentBlock:(RequestExtentBlock)extentBlock compeletion:(ResponseBlock)compeletion;

@end

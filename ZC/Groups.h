//
//  Group.h
//  oc_demo
//
//  Created by zhangxinwei on 15/11/24.
//  Copyright © 2015年 zhangxinwei. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Groups : NSObject
@property (nonatomic, copy) NSString *groupPath;

@property (nonatomic, strong) NSMutableDictionary *mGroups;

@property (nonatomic, strong) NSData *groupData;

@property (nonatomic, strong) NSMutableArray *groupInfos;

@property (nonatomic, strong) NSMutableData *groupFooter;

@property (nonatomic, copy) NSString *groupName;
@property (nonatomic) int32_t groupId;
@property (nonatomic, strong) NSMutableDictionary *groupInfo; //{"id":"", "name":"", "keys":""}
@property (nonatomic, strong) NSMutableArray *groups;

+ (Groups *)share;

- (int32_t)getGroupIdByName:(NSString *)name;
- (BOOL)addKey:(NSString *)key groupName:(NSString *)groupName;
- (int8_t)getKeyIndex:(NSString *)key groupName:(NSString *)groupName;
@end

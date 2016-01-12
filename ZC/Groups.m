//
//  Group.m
//  oc_demo
//
//  Created by zhangxinwei on 15/11/24.
//  Copyright © 2015年 zhangxinwei. All rights reserved.
//

#import "Groups.h"

@implementation Groups
+ (Groups *)share {
    static Groups *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[Groups alloc] init];
        
        NSString *groupPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
        instance.groupPath = [groupPath stringByAppendingPathComponent:@"groups.data"];
        
        NSFileManager *fm = [NSFileManager defaultManager];
        
        if ([fm fileExistsAtPath:instance.groupPath]) {
            NSFileHandle *fh = [NSFileHandle fileHandleForUpdatingAtPath:instance.groupPath];
            instance.groupData = [fh readDataToEndOfFile];
        }else {
            int16_t count = 0;
            int32_t len = 0;
            int16_t lastGroupId = 0;
            
            NSMutableData *md = [[NSMutableData alloc] init];
            [md appendBytes:&lastGroupId length:sizeof(int16_t)];
            [md appendBytes:&len length:sizeof(int32_t)];
            [md appendBytes:&count length:sizeof(int16_t)];
            
            instance.groupData = md;
        }
        
    });
    return instance;
}

- (BOOL)addGroup:(NSString *)groupName keys:(NSString *)keys {
    
    if (groupName.length <= 0) {
        return false;
    }
    
    //check groupName
//    [Groups share].groupData 
    
    int32_t totalLen = (int32_t)[Groups share].groupData.length;
    //delete file
    NSString *path = [Groups share].groupPath;
    [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
    NSFileHandle *fh = [NSFileHandle fileHandleForUpdatingAtPath:path];
  
    // old footer len
    int32_t len;
    [[Groups share].groupData getBytes:&len range:NSMakeRange(totalLen - 6, sizeof(int32_t))];

    //old count
    int16_t count;
    [[Groups share].groupData getBytes:&count range:NSMakeRange(totalLen-2, sizeof(int16_t))];
    
    //old last group id
    int16_t lastGroupId;
    [[Groups share].groupData getBytes:&lastGroupId range:NSMakeRange(8, sizeof(int16_t))];
    
    // old groupinfo
    NSData *oldGroupInfoData;
    if (len <= 8) {
        oldGroupInfoData = [[NSData alloc] init];
    }else {
        oldGroupInfoData = [[Groups share].groupData subdataWithRange:NSMakeRange(0, totalLen - len)];
    }
    
    //write old groupInfo
    [oldGroupInfoData writeToFile:path atomically:YES];
    [fh writeData:oldGroupInfoData];
    
    //write this groupInfo
    int32_t thisOffset = (int32_t)[fh seekToEndOfFile];
    NSMutableData *thisGroupInfo = [[NSMutableData alloc] init];
    if (keys.length <= 0) {
        int8_t groupNameLen = (int8_t)[groupName dataUsingEncoding:NSUTF8StringEncoding].length;
        int32_t keysLen = 0;
        int16_t groupId = lastGroupId + 1;
        
        [thisGroupInfo appendBytes:&groupNameLen length:sizeof(int8_t)];
        [thisGroupInfo appendBytes:&keysLen length:sizeof(int32_t)];
        [thisGroupInfo appendBytes:&groupId length:sizeof(int16_t)];
        [thisGroupInfo appendData:[groupName dataUsingEncoding:NSUTF8StringEncoding]];
        
    }else {
        int8_t groupNameLen = (int8_t)[groupName dataUsingEncoding:NSUTF8StringEncoding].length;
        int32_t keysLen = (int32_t)[keys dataUsingEncoding:NSUTF8StringEncoding].length;
        int16_t groupId = lastGroupId + 1;

        [thisGroupInfo appendBytes:&groupNameLen length:sizeof(int8_t)];
        [thisGroupInfo appendBytes:&keysLen length:sizeof(int32_t)];
        [thisGroupInfo appendBytes:&groupId length:sizeof(int16_t)];
        [thisGroupInfo appendData:[groupName dataUsingEncoding:NSUTF8StringEncoding]];
        [thisGroupInfo appendData:[keys dataUsingEncoding:NSUTF8StringEncoding]];
    }
    [fh writeData:thisGroupInfo];
    
    //write footer
    NSMutableData *footer = [[NSMutableData alloc] init];
    
    // old offsets
    if (len <= 8) {
        [footer appendBytes:&thisOffset length:sizeof(int32_t)];
    }else {
        NSData *d = [[Groups share].groupData subdataWithRange:NSMakeRange(totalLen - len, len - 8)];
        [footer appendData:d];
    }
    
    lastGroupId += 1;
    len = (int32_t)footer.length + 8;
    count += 1;
    [footer appendBytes:&lastGroupId length:sizeof(int16_t)];
    [footer appendBytes:&len length:sizeof(int32_t)];
    [footer appendBytes:&count length:sizeof(int16_t)];
    
    //write footer
    [fh writeData:footer];
    
    // update group data
    [Groups share].groupData = [fh readDataToEndOfFile];
    
    [fh closeFile];
    
    return true;
}

//- (BOOL)addKey:(NSString *)key groupName:(NSString *)groupName {
//    if (!_groups) {
//        [self addGroup:groupName];
//    }
//    
//    //check group
//    __block BOOL isHave1 = false;
//    [_groups enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
//        NSDictionary *d = obj;
//        NSString *gn = [d valueForKey:@"groupName"];
//        if ([gn isEqualToString:groupName]) {
//            stop = YES;
//            isHave1 = YES;
//        }
//    }];
//    
//    if (!isHave1) {
//        NSLog(@"group不存在");
//        [self addGroup:groupName];
//    }
//    
//    // check key
//    __block BOOL isHave = false;
//    [_groups enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
//        NSDictionary *d = obj;
//        NSString *gn = [d valueForKey:@"groupName"];
//        if ([gn isEqualToString:groupName]) {
//            NSArray *a = [d valueForKey:@"keys"];
//            if ([a containsObject:key]) {
//                stop = YES;
//                isHave = YES;
//            }
//        }
//    }];
//    
//    if (isHave) {
//        NSLog(@"key已存在");
//        return false;
//    }
//    
//    // add key
//    __block NSMutableArray *ma;
//    [_groups enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
//        NSMutableDictionary *d = obj;
//        NSString *gn = [d valueForKey:@"groupName"];
//        if ([gn isEqualToString:groupName]) {
//            NSArray *a = [d valueForKey:@"keys"];
//            ma = [a mutableCopy];
//            [ma addObject:key];
//            [d setValue:ma forKey:@"keys"];
//            stop = YES;
//        }
//    }];
//    
//    return true;
//}
//
//- (Byte)getKeyIndex:(NSString *)key groupName:(NSString *)groupName {
//    __block Byte index;
//
//    [_groups enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
//        NSDictionary *d = obj;
//        NSString *gn = [d valueForKey:@"groupName"];
//        if ([gn isEqualToString:groupName]) {
//            NSArray *a = [d valueForKey:@"keys"];
//            __block bool isHave;
//            [a enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
//                NSString *keyName = obj;
//                if ([keyName isEqualToString:key]) {
//                    index = idx;
//                    stop = YES;
//                    isHave = YES;
//                }
//            }];
//            stop = isHave;
//        }
//    }];
//    
//    return index;
//}
@end

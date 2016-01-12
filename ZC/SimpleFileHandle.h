//
//  SimpleFileHandle.h
//  oc_demo
//
//  Created by zhangxinwei on 15/12/1.
//  Copyright © 2015年 zhangxinwei. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SimpleFileHandle : NSObject

+ (SimpleFileHandle *)fileHandleForUpdatingAtPath:(NSString *)path;

- (NSData *)readDataToEndOfFile;

- (void)seekToFileOffset:(int64_t)offset;

- (int64_t)seekToEndOfFile;

- (void)writeData:(NSData *)data;

- (NSData *)readDataOfLength:(int32_t)len;

- (void)closeFile;

@end

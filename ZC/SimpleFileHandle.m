//
//  SimpleFileHandle.m
//  oc_demo
//
//  Created by zhangxinwei on 15/12/1.
//  Copyright © 2015年 zhangxinwei. All rights reserved.
//

#import "SimpleFileHandle.h"

@interface SimpleFileHandle ()
@property (nonatomic) FILE *f;
@end
    
@implementation SimpleFileHandle
+ (SimpleFileHandle *)fileHandleForUpdatingAtPath:(NSString *)path {
    static SimpleFileHandle *instance = nil;
    
    FILE *f;
    if (!instance) {
        if ((f = fopen([path UTF8String], "rwab")) == NULL) {
            NSLog(@"open file error");
            return nil;
        }
        instance = [[SimpleFileHandle alloc] init];
        instance.f = f;
    }
    return instance;
}

- (NSData *)readDataToEndOfFile {
    fseek(_f, 0, SEEK_END);
    long len = ftell(_f);
    char *ret = malloc(len);
    fseek(_f, 0, SEEK_SET);
    fread(ret, 1, len, _f);
    
    NSData *data = [NSData dataWithBytes:ret length:len];
    
    return data;
}

- (void)seekToFileOffset:(int64_t)offset {
    fseek(_f, offset, SEEK_SET);
}

- (int64_t)seekToEndOfFile {
    fseek(_f, 0, SEEK_END);
    int64_t len = ftell(_f);
    return len;
}

- (void)writeData:(NSData *)data {
    int32_t len = (int32_t)data.length;
    Byte *bs = [data bytes];
    fwrite(&bs, len, 1, _f);
}

- (NSData *)readDataOfLength:(int32_t)len {
    char *ret = malloc(len);
    fread(ret, 1, len, _f);
    
    NSData *data = [NSData dataWithBytes:ret length:len];
    
    return data;
}

- (void)closeFile {
    //    fclose(f);
}

@end
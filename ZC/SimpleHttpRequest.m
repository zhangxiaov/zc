//
//  SimpleHttpRequest.m
//  oc_demo
//
//  Created by zhangxinwei on 15/12/3.
//  Copyright © 2015年 zhangxinwei. All rights reserved.
//

#import "SimpleHttpRequest.h"
#include <netdb.h>
#include <arpa/inet.h>

@implementation SimpleHttpRequest

+ (void)postWithParams:(NSDictionary *)dict path:(NSString *)urlPath compeletion:(ResponseBlock)compeletion {
    NSURL *url = [[NSURL alloc] initWithString:urlPath];
    
    //ip
    NSString *hostName = [url host];
    char ip[20];
    struct hostent *ent = gethostbyname([hostName UTF8String]);
    memcpy(&ip, inet_ntoa(*((struct in_addr *)ent->h_addr_list[0])), 20);
    NSString *IP = [NSString stringWithUTF8String:ip];
    
    // port
    NSNumber *port = [url port];
    if (port == nil) {
        NSString *p = [url scheme];
        if ([p isEqualToString:@"http"]) {
            port = [NSNumber numberWithInt:80];
        }else if([p isEqualToString:@"https"]) {
            port = [NSNumber numberWithInt:443];
        }else {
            NSLog(@"%@ no port", p);
        }
    }
    NSString *path = [url path];

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = [port intValue];
    
    struct addrinfo hints, *serverInfo;
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo([IP UTF8String], [[port stringValue] UTF8String], &hints, &serverInfo);
    if (err) {
        NSLog(@"%@", [NSString stringWithUTF8String:strerror(errno)]);
        return;
    }
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd <= 0) {
        NSLog(@"%@", [NSString stringWithUTF8String:strerror(errno)]);
        return;
    }
    
    if (connect(fd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
        NSLog(@"%@", [NSString stringWithUTF8String:strerror(errno)]);
        return;
    }
    
    //set header
    NSArray *keys = [dict allKeys];
    NSString *header = [[NSString alloc] init];
    NSString *content = [[NSString alloc] init];
    for (NSString *key in keys) {
        content = [content stringByAppendingString:key];
        content = [content stringByAppendingString:@"="];
        content = [content stringByAppendingString:[dict valueForKey:key]];
        content = [content stringByAppendingString:@"&"];
    }
    content = [content stringByAppendingString:@"\n"];
    
    int contentLen = (int)content.length;
    
    header = [header stringByAppendingString:@"POST "];
    header = [header stringByAppendingString:path];
    header = [header stringByAppendingString:@" HTTP/1.1\n"];
    
    header = [header stringByAppendingString:@"Host: "];
    header = [header stringByAppendingString:hostName];
    header = [header stringByAppendingString:@"\n"];
    
    header = [header stringByAppendingString:@"Content-Length: "];
    header = [header stringByAppendingString:[NSString stringWithFormat:@"%d", contentLen]];
    header = [header stringByAppendingString:@"\n\n"];
    
    header = [header stringByAppendingString:content];
    
    // send header
    int len = (int)header.length;
    const char *buf = [header UTF8String];
    long count = send(fd , buf, len, 0);
    if (count < 0) {
        NSLog(@"%@", [NSString stringWithUTF8String:strerror(errno)]);
        return;
    }
    
    //log
    NSLog(@"%@\n", urlPath);
    for (NSString *key in keys) {
        NSLog(@"\t%@ = %@ \n", key, [dict valueForKey:key]);
    }
    
    //getdata
    NSMutableData *md = [[NSMutableData alloc] init];

    int count_get = 0;
    int bufLen = 5000;
    do {
        char buf[bufLen];
        count_get = recv(fd, &buf, sizeof(buf), 0);
        if (count_get <= 0) {
            NSLog(@"%@ or no data", [NSString stringWithUTF8String:strerror(errno)]);
            break;
        }
        
        [md appendBytes:buf length:count_get];

    }while (count_get > 0);

    NSLog(@"%ld", md.length);
    NSLog(@"%@", [[NSString alloc] initWithData:md encoding:NSUTF8StringEncoding]);
    if(compeletion)
        compeletion(md);
    
//error:
//    if (error && errno) {
//        NSError *e =
//        [[NSError alloc] initWithDomain:@"SimpleClientSockDomain" code:errno userInfo:[NSDictionary dictionaryWithObject:[NSString stringWithUTF8String:strerror(errno)] forKey:NSLocalizedDescriptionKey]];
//        error(e);
//    }
    
    close(fd);
    return;
}

+ (void)postWithParams:(NSDictionary *)dict path:(NSString *)path extentBlock:(RequestExtentBlock)extentBlock compeletion:(ResponseBlock)compeletion {
    
}

int	connect_timeout(int sockfd, const struct sockaddr *address, socklen_t address_len, long timeout) {

    return 0;
}

@end

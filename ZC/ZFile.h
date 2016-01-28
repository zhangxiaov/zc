//
//  ZFile.h
//  ZC
//
//  Created by zhangxinwei on 16/1/28.
//  Copyright © 2016年 张新伟. All rights reserved.
//

#ifndef ZFile_h
#define ZFile_h

#include <stdio.h>
#include "ZC.h"

void zfileCreate(char* fileName);

//读取len子节
char* zfileRead(char* self, int startPos, int len);
bool zfileWrite(char* self, int startPos, void* valuePtr);

#endif /* ZFile_h */

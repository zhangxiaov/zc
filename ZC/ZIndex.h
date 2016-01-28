//
//  ZIndex.h
//  ZC
//
//  Created by zhangxinwei on 16/1/28.
//  Copyright © 2016年 张新伟. All rights reserved.
//

#ifndef ZIndex_h
#define ZIndex_h

#include <stdio.h>
#include "ZArray.h"

//static ZArray* indexArray;

typedef struct _ZIndex ZIndex;

struct _ZIndex {
    int type;
    int typeSize;
    char* name;
    int valueSpace;
};

//索引文件名 数组
ZArray* zindexArray();

#endif /* ZIndex_h */

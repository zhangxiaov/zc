//
//  ZDB.h
//  ZC
//
//  Created by 张新伟 on 16/1/12.
//  Copyright © 2016年 张新伟. All rights reserved.
//

#ifndef ZDB_h
#define ZDB_h

#include <stdio.h>
#include "ZC.h"
#include "ZJson.h"


typedef struct _zrecord zrecord;
typedef struct _zdb zdb;

struct _zdb {
    void* mmap;
};

struct _zrecord {
    char jsonStr[6*1024];
};


#define recordSize 6*1024;

bool zdbCreate(char* fileName, int size);
void zdbClose(zdb* self, int size);
zdb* zdbInit(char* fileName);
char* zdbReadToString(zdb* self, int pos);
ZJson* zdbReadToJson(zdb* self, int pos);
void zdbUpdate(zdb* self, int pos, char* val);
int zdbAdd(zdb* self, char* val);

#endif /* ZDB_h */

//
//  ZIndex.c
//  ZC
//
//  Created by zhangxinwei on 16/1/28.
//  Copyright © 2016年 张新伟. All rights reserved.
//

#include "ZIndex.h"
#include "CString.h"
#include "ZFile.h"

//字段之索引，便于根据字段 对记录分组,
//索引文件名：index_xxxx，格式：
//8byte,记录id1，记录id2，，，
//整型8byte，字符 11byte，大小随值,值1，值2，，，，

static int intvalueSpace = 8;
static int strvalueSpace = 11;

//创建一个索引文件 self == indexArray
void zindexCreate(ZArray* indexArray, char* indexName, int valueType) {
    indexArray = zindexArray(); //判空
    
    //1
    ZIndex* index = zindexInit();
    index->name = indexName;
    index->valueSpace = valueType;
    
    zarrayAdd(indexArray, index);
    //2
    
    char* fileName = csAppend("index_", indexName);
    zfileCreate(fileName);
    
    if (valueType == typeInt) {
        zfileWrite(fileName, 0, &intvalueSpace);
    }else if (valueType == typeString) {
        zfileWrite(fileName, 0, &strvalueSpace);
    }
}


//追加，当新增一条记录含此字段
void zindexAppend(char* self, void* value) {
    
}

//改，当修改纪录此字段值
void zindexUpdate(char* self, void* value) {
    
}

//删某一纪录
void zindexDel(char* self) {
    
}

//销毁文件
void zindexDestory(char* self) {
    
}

ZIndex* zindexInit() {
    ZIndex* index = (ZIndex*)malloc(sizeof(ZIndex));
    return index;
}

ZArray* zindexArray(ZArray* indexArray) {
    // dir_db/db_xxxx
    //  dir_db/index_xxx
    if (indexArray != NULL) {
        return indexArray;
    }
    
    ZArray* array = zarrayInit(true);
    
    ZArray* fileNames = zarrayFileNames("/tmp/db/");
    int count = fileNames->len;
    for (int i = 0; i < count; i++) {
        char* name = zarrayGet(fileNames, i);
        char* n = csSeparateToEnd(name, "_");
        ZIndex* index = zindexInit();
        index->name = n;
        index->valueSpace = *zfileRead(name, 0, 1); //读取一个字节，其值为valuespace
        
        zarrayAdd(array, index);
    }
    
    return array;
}


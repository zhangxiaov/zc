//
//  ZArray.c
//  oc_demo
//
//  Created by 张新伟 on 15/12/13.
//  Copyright (c) 2015年 zhangxinwei. All rights reserved.
//

#include "ZArray.h"
#include <string.h>
#include "CString.h"
#include <stdlib.h>
#include <stdarg.h>
#include "ZC.h"
#include <string.h>

// data 仅存 指针类型
struct _ZArray {
    int type;
    int typeSize;
    zptr data;
    int len;
    int pos;
    bool isDynamic;
};

ZArray* zarrayInit(bool isDynamic) {
    ZArray* array = (ZArray*)malloc(sizeof(ZArray));
    array->type = typeZArray;
    array->isDynamic = isDynamic;
    array->data = NULL;
    array->len = 0;
    return array;
}

ZArray* zarrayInitWithVal(int n, void* val1,...) {
    ZArray* array = (ZArray*)malloc(sizeof(ZArray));
    array->type = 12;
    array->data = malloc(n*8);
    
    va_list va;
    va_start(va, val1);
    
    for (int i = 0; i < n; i++) {
        array->data += i;
        array->data = val1;
        val1 = va_arg(va, void*);
    }
    
    return array;
}

void* zarrayGet(ZArray* self, int index) {
    if (index >= self->len) {
        printf("zarray index > len\n");
        return NULL;
    }
    zptr data = self->data;
    zptr d = data+index;
    return *d;
}

void zarrayAdd(ZArray* self, void* val) {
    if (val == NULL) {
        return;
    }
    if (self->isDynamic == false && self->pos == self->len - 1) {
        printf("array pos == len\n");
        return;
    }
    if (self->data == NULL) {
        zptr d = malloc(sizeof(zptr));
        *d = val;
        self->data = d;
        ++self->len;
    }else {
        ++self->len;
        zptr newD = malloc(sizeof(zptr)*(self->len));
        memcpy(newD, self->data, sizeof(zptr)*(self->len-1));
        *(newD+self->len-1) = val;
        self->data = newD;
    }
}

void zarraySet(ZArray* self,int index, void* val) {
    
}

char* zarrayToString(ZArray* self) {
    char* str = (char*)malloc(1);
    str = csAppend("", "[");
    
    int capacity = self->len;
    for (int i = 0; i < capacity; i++) {
        zptr data = self->data + i;
        // val
        ZStruct* cz = (ZStruct*)*data;
        int type = cz->type;
        switch (type) {
            case typeZArray:
                str = csAppend(str, zarrayToString((ZArray*)cz));
                str = csAppend(str, ",");
                break;
            case typeZMap:
                str = csAppend(str, zmapToString(((ZMap*)cz)));
                str = csAppend(str, ",");
                break;
            default:
                str = csAppend(str, "\"");
                str = csAppend(str, ((char*)cz));
                str = csAppend(str, "\",");
                break;
        }
    }
    
    if (strlen(str) == 1) {
        str = csAppend(str, "]");
    }else
        str = csReplaceCharAtLast(str, ']');
    return str;
}

void main_array(){
//void main() {

    
    ZArray* array = zarrayInit(true);

    zarrayAdd(array, "张新伟");
    zarrayAdd(array, "25");
    zarrayAdd(array, "nojob");
    
    ZArray* innerArray = zarrayInit(true);
    zarrayAdd(innerArray, "xx");
    
    ZMap* innerMap = zmapInit();
    zmapPut(innerMap, "innerK", "innerV");
    
    zarrayAdd(array, innerArray);
    zarrayAdd(array, innerMap);
    
    int i = 11;
//    zarrayAdd(array, &i);
    
    printf("%s \n", zarrayGet(array, 0));
    printf("%s \n", zarrayGet(array, 1));
    printf("%s \n", zarrayGet(array, 2));
    zptr addr = zarrayGet(array, 3);
//    printf("%d \n", *addr);
    
    printf("array -> json\n %s \n", zarrayToString(array));
}
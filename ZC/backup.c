////
////  backup.c
////  ZC
////
////  Created by 张新伟 on 16/1/3.
////  Copyright © 2016年 张新伟. All rights reserved.
////
//
//#include "backup.h"
////
////  ZJson.c
////  oc_demo
////
////  Created by zhangxinwei on 15/12/7.
////  Copyright © 2015年 zhangxinwei. All rights reserved.
////
//
//#include "ZJson.h"
//#include <stdarg.h>
//#include <sys/malloc.h>
//#include <string.h>
//#include "ZTime.h"
//#include "CString.h"
//#include "ZStack.h"
//#include "ZMap.h"
//#include "ZArray.h"
//
//
//
//typedef struct _zjsonStr zjsonStr;
//
//struct _zjsonStr {
//    char* str;
//    int nextTokenIndex;
//};
//
//int hasStatus(int expectedStatus);
//int zjsonStrType(zjsonStr* jsonStr);
//void* zjsonStrReadNumber(zjsonStr* jsonStr);
//void* zjsonStrReadBoolean(zjsonStr* jsonStr);
//void zjsonStrReadNull(zjsonStr* jsonStr);
//char* zjsonStrReadString(zjsonStr* self);
//
//#define DOUBLE(a) ({\
//__typeof__(a) _x_in_DOUBLE = (a);\
//_x_in_DOUBLE + _x_in_DOUBLE;\
//})
//// type = 1 obj, = 0 array
//struct _CZ {
//    char type;
//    int typeSize;
//};
//
//// type = 11 obj, = 10 array
//struct _ZJson {
//    char type;
//    int typeSize;
//    KV* data;
//    char count;
//};
//
//struct _KV {
//    char* key;
//    void* val;
//    KV* next;
//};
//
//typedef struct _zjsonNode zjsonNode;
//typedef struct _ZJson2 ZJson2;
//
//struct _ZJson2 {
//    int type;
//    int typeSize;
//    int size;
//    int count;
//    zjsonNode* data;
//    char* originString;
//};
//
//struct _zjsonNode {
//    char* key;
//    void* val;
//    zjsonNode* next;
//};
//
//ZJson* zjsonArrayInitWithVal(int n,void* v1,...) {
//    ZJson* jsonArray = (ZJson*)malloc(sizeof(ZJson));
//    jsonArray->type = ZJSONARRAY;
//    jsonArray->data = (KV*)malloc(sizeof(KV));
//    jsonArray->count = n;
//    
//    va_list argp;
//    va_start(argp, v1);
//    
//    KV* next = jsonArray->data;
//    
//    for (int i = 0; i < n; i++) {
//        KV* kv = next;
//        kv->val = v1;
//        
//        next = (KV*)malloc(sizeof(KV));
//        next->next = NULL;
//        kv->next = next;
//        
//        v1 = va_arg(argp, void*);
//    }
//    
//    return jsonArray;
//}
//
//ZJson* zjsonInitWithJson(ZJson* json) {
//    
//    return 0;
//}
//
//ZJson2* zjsonInitWithString(char* jsonStr) {
//    ZJson2* json = (ZJson2*)malloc(sizeof(ZJson2));
//    json->type = ZJSON;
//    json->data = (zjsonNode*)malloc(sizeof(zjsonNode));
//    json->originString = jsonStr;
//    char c = (char)(*jsonStr);
//    
//    zjsonStr* zjstr = (zjsonStr*)malloc(sizeof(zjsonStr));
//    zjstr->str = jsonStr;
//    zjstr->nextTokenIndex = 0;
//    
//    int i = 0;
//    size_t len = strlen(jsonStr);
//    
//    ZStack* stack = zstackInit(10, typePtr);
//    
//    int status = STATUS_EXPECT_SINGLE_VALUE | STATUS_EXPECT_BEGIN_OBJECT | STATUS_EXPECT_BEGIN_ARRAY;
//    while (i < len) {
//        
//        int type = zjsonStrType(zjstr);
//        switch (type) {
//            case idnumber:
//                if (hasStatus(STATUS_EXPECT_SINGLE_VALUE)) {//single number
//                    zjsonNumber number = zjsonStrReadNumber(zjstr);
//                    zstackPush(stack, number);
//                    status = STATUS_EXPECT_END_DOCUMENT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_OBJECT_VALUE)) {
//                    zjsonNumber number = zjsonStrReadNumber(zjstr);
//                    char* key = zstackPop(stack);
//                    zmapPut(zstackPeek(stack), key, number);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_OBJECT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_ARRAY_VALUE)) {
//                    zjsonNumber number = zjsonStrReadNumber(zjstr);
//                    zarrayAdd(zstackPeek(stack), number);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_ARRAY;
//                    continue;
//                }
//                printf("Unexpected umber\n");
//                break;
//            case idbool:
//                if (hasStatus(STATUS_EXPECT_SINGLE_VALUE)) {// single boolean:
//                    zjsonBoolean boolean = zjsonStrReadBoolean(zjstr);
//                    zstackPush(stack, boolean);
//                    status = STATUS_EXPECT_END_DOCUMENT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_OBJECT_VALUE)) {
//                    zjsonBoolean boolean = zjsonStrReadBoolean(zjstr);
//                    
//                    char* key = zstackPop(stack);
//                    zmapPut(zstackPeek(stack), key, boolean);
//                    
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_OBJECT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_ARRAY_VALUE)) {
//                    zjsonBoolean boolean = zjsonStrReadBoolean(zjstr);
//                    zarrayAdd(zstackPeek(stack), boolean);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_ARRAY;
//                    continue;
//                }
//                printf("Unexpected bool\n");
//                break;
//            case idstring:
//                if (hasStatus(STATUS_EXPECT_SINGLE_VALUE)) {// single string:
//                    char* str = zjsonStrReadString(zjstr);
//                    zstackPush(stack, str);
//                    status = STATUS_EXPECT_END_DOCUMENT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_OBJECT_KEY)) {
//                    char* str = zjsonStrReadString(zjstr);
//                    zstackPush(stack, str);
//                    status = STATUS_EXPECT_COLON;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_OBJECT_VALUE)) {
//                    char* str = zjsonStrReadString(zjstr);
//                    char* key = zstackPop(stack);
//                    zmapPut(zstackPeek(stack), key, str);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_OBJECT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_ARRAY_VALUE)) {
//                    char* str = zjsonStrReadString(zjstr);
//                    zarrayAdd(zstackPeek(stack), str);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_ARRAY;
//                    continue;
//                }
//                printf("Unexpected string \n");
//                break;
//            case idnull:
//                if (hasStatus(STATUS_EXPECT_SINGLE_VALUE)) {// single null:
//                    zjsonStrReadNull(zjstr);
//                    zstackPush(stack, NULL);
//                    status = STATUS_EXPECT_END_DOCUMENT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_OBJECT_VALUE)) {
//                    zjsonStrReadNull(zjstr);
//                    char* key = zstackPop(stack);
//                    zmapPut(zstackPeek(stack), key, NULL);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_OBJECT;
//                    continue;
//                }
//                if (hasStatus(STATUS_EXPECT_ARRAY_VALUE)) {
//                    zjsonStrReadNull(zjstr);
//                    zarrayAdd(zstackPeek(stack), NULL);
//                    status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_ARRAY;
//                    continue;
//                }
//                printf("Unexpected null\n");
//                break;
//            case idobjbegin:// {
//                if (hasStatus(STATUS_EXPECT_BEGIN_OBJECT)) {
//                    zstackPush(stack, zmapInit());
//                    status = STATUS_EXPECT_OBJECT_KEY | STATUS_EXPECT_BEGIN_OBJECT | STATUS_EXPECT_END_OBJECT;
//                    continue;
//                }
//                printf("Unexpected '{'\n");
//                break;
//            case idobjend:// }
//                if (hasStatus(STATUS_EXPECT_END_OBJECT)) {
//                    ZMap* map = zstackPop(stack);
//                    if (zstackEmpty(stack)) {//root object
//                        zstackPush(stack, map);
//                        status = STATUS_EXPECT_END_DOCUMENT;
//                        continue;
//                    }
//                    
//                    int type = zstackPreValueType(stack);
//                    if (type == TYPE_OBJECT_KEY) {
//                        char* key = zstackPop(stack);
//                        zmapPut(zstackPeek(stack), key, map);
//                        status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_OBJECT;
//                        continue;
//                    }
//                    if (type == TYPE_ARRAY) {
//                        zarrayAdd(zstackPeek(stack), map);
//                        status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_ARRAY;
//                        continue;
//                    }
//                }
//                printf("Unexpected '}'\n");
//                break;
//            case idlistbegin:// [
//                if (hasStatus(STATUS_EXPECT_BEGIN_ARRAY)) {
//                    zstackPush(stack, zarrayInit());
//                    status = STATUS_EXPECT_ARRAY_VALUE | STATUS_EXPECT_BEGIN_OBJECT | STATUS_EXPECT_BEGIN_ARRAY | STATUS_EXPECT_END_ARRAY;
//                    continue;
//                }
//                printf("Unexpected '['\n");
//                break;
//            case idlistend: // ]
//                if (hasStatus(STATUS_EXPECT_END_ARRAY)) {
//                    ZArray* array = zstackPop(stack);
//                    if (zstackEmpty(stack)) {
//                        zstackPush(stack, array);
//                        status = STATUS_EXPECT_END_DOCUMENT;
//                        continue;
//                    }
//                    
//                    int type = zstackPreValueType(stack);
//                    if (type == TYPE_OBJECT_KEY) {// key: [ CURRENT ] ,}
//                        char* key = zstackPop(stack);
//                        zmapPut(zstackPeek(stack), key, array);
//                        status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_OBJECT;
//                        continue;
//                    }
//                    if (type == TYPE_ARRAY) {// xx, xx, [CURRENT] ,]
//                        zarrayAdd(zstackPeek(stack), array);
//                        status = STATUS_EXPECT_COMMA | STATUS_EXPECT_END_ARRAY;
//                        continue;
//                    }
//                }
//                printf("Unexpected char ']'\n");
//                break;
//                
//            case idcolon://:
//                if (status == STATUS_EXPECT_COLON) {
//                    status = STATUS_EXPECT_OBJECT_VALUE | STATUS_EXPECT_BEGIN_OBJECT | STATUS_EXPECT_BEGIN_ARRAY;
//                    continue;
//                }
//                printf("Unexpected colon char\n");
//                break;
//            case idcomma://,
//                if (hasStatus(STATUS_EXPECT_COMMA)) {
//                    if (hasStatus(STATUS_EXPECT_END_OBJECT)) {
//                        status = STATUS_EXPECT_OBJECT_KEY;
//                        continue;
//                    }
//                    if (hasStatus(STATUS_EXPECT_END_ARRAY)) {
//                        status = STATUS_EXPECT_ARRAY_VALUE | STATUS_EXPECT_BEGIN_ARRAY | STATUS_EXPECT_BEGIN_OBJECT;
//                        continue;
//                    }
//                }
//                printf("Unexpected comma char\n");
//                break;
//            case idend:
//                if (hasStatus(STATUS_EXPECT_END_DOCUMENT)) {
//                    void* json = zstackPop(stack);
//                    if (zstackEmpty(stack)) {
//                        return json;
//                    }
//                }
//                printf("Unexpected EOF \n");
//                break;
//            default:
//                break;
//        }
//    }
//    
//    return json;
//}
//
//int hasStatus(int expectedStatus) {
//    int status = STATUS_EXPECT_SINGLE_VALUE | STATUS_EXPECT_BEGIN_OBJECT | STATUS_EXPECT_BEGIN_ARRAY;
//    return ((status & expectedStatus) > 0);
//}
//
//int zjsonStrType(zjsonStr* jsonStr) {
//    
//    return 0;
//}
//
//void* zjsonStrReadNumber(zjsonStr* jsonStr) {
//    
//    return NULL;
//}
//
//void* zjsonStrReadBoolean(zjsonStr* jsonStr) {
//    
//    return NULL;
//}
//
//void zjsonStrReadNull(zjsonStr* jsonStr) {
//    
//}
//
//char* zjsonStrReadString(zjsonStr* self) {
//    return NULL;
//}
//
//ZJson* zjsonInitWithKV(int n, void* k1,...) {
//    ZJson* json = (ZJson*)malloc(sizeof(ZJson));
//    json->type = ZJSON;
//    json->data = (KV*)malloc(sizeof(KV));
//    json->count = n;
//    
//    va_list argp;
//    va_start(argp, k1);
//    
//    KV* next = json->data;
//    
//    for (int i = 0; i < n; i++) {
//        KV* kv = next;
//        
//        // key
//        CZ* k_obj = (CZ*)k1;
//        kv->key = ((zs*)k_obj)->val;
//        
//        // val
//        k1 = va_arg(argp, void*);
//        kv->val = k1;
//        
//        next = (KV*)malloc(sizeof(KV));
//        next->next = NULL;
//        kv->next = next;
//        
//        k1 = va_arg(argp, void*);
//    }
//    
//    return json;
//}
//
//char* zjsonArrayToString(ZJson* jsonArray) {
//    char* str = "[";
//    
//    char type = jsonArray->type;
//    int count = jsonArray->count;
//    KV* data = jsonArray->data;
//    
//    KV* kv = data;
//    for (int i = 0; i < count; i++) {
//        char* k = kv->key;
//        
//        // val
//        CZ* cz = (CZ*)kv->val;
//        char type = cz->type;
//        switch (type) {
//            case ZI:
//                str = csAppendInt(str, ((zi*)cz)->val);
//                str = csAppend(str, ",");
//                break;
//            case ZJSONARRAY:
//                str = csAppend(str, zjsonArrayToString(((ZJson*)cz)));
//                break;
//            case ZJSON:
//                str = csAppend(str, zjsonToString(((ZJson*)cz)));
//                break;
//            case ZS:
//                str = csAppend(str, "\"");
//                str = csAppend(str, ((zs*)cz)->val);
//                str = csAppend(str, "\",");
//                break;
//            default:
//                break;
//        }
//        
//        kv = kv->next;
//    }
//    
//    str = csReplaceCharAtLast(str, ']');
//    str = csAppend(str, ",");
//    return str;
//}
//
//char* zjsonToString(ZJson* json) {
//    char* str = "{";
//    
//    char type = json->type;
//    int count = json->count;
//    KV* data = json->data;
//    
//    // array
//    if (type == ZJSONARRAY) {
//        zjsonArrayToString(json);
//    }else if (type == ZJSON) {//kv
//        
//        KV* kv = data;
//        for (int i = 0; i < count; i++) {
//            char* k = kv->key;
//            
//            // key
//            str = csAppend(str, "\"");
//            str = csAppend(str, k);
//            str = csAppend(str, "\"");
//            
//            //            printf("%s\n", str);
//            
//            str = csAppend(str, "\:");
//            // val
//            CZ* cz = (CZ*)kv->val;
//            char type = cz->type;
//            switch (type) {
//                case ZI:
//                    str = csAppendInt(str, ((zi*)cz)->val);
//                    str = csAppend(str, ",");
//                    break;
//                case ZJSONARRAY:
//                    str = csAppend(str, zjsonArrayToString(((ZJson*)cz)));
//                    break;
//                case ZJSON:
//                    str = csAppend(str, zjsonToString(((ZJson*)cz)));
//                    break;
//                case ZS:
//                    str = csAppend(str, "\"");
//                    str = csAppend(str, ((zs*)cz)->val);
//                    str = csAppend(str, "\",");
//                    break;
//                default:
//                    break;
//            }
//            
//            kv = kv->next;
//        }
//    }
//    
//    str = csReplaceCharAtLast(str, '}');
//    return str;
//}
//
//void main_zjson() {
//    //void main(){
//    
//    uint64_t s = ztimeSince1970();
//    ZJson* json = zjsonInitWithKV(4, zs("name"), zs("张新伟"), zs("age"), zi(26), zs("mobile"), zs("15210743874"), zs("test"), zjsonArrayInitWithVal(2, zs("zhangxinwei"), zi(100)));
//    
//    char* str = zjsonToString(json);
//    char* str2 = zjsonToString(json);
//    
//    printf("time := %llu\n", ztimeSince1970() - s);
//    printf("%s\n", str);
//    printf("%s\n", str2);
//    
//    printf("%d\n", strlen("正"));
//    
//    
//    char* jsonString = "{\"name\":\"张新伟\",\"age\":25,\"job\":\"nojob\",\"array\":[11,true,\"test\",{\"arrayInnerK\":\"arrayInnerv\"}]}";
//    printf("%c \n", *jsonString);
//    ZJson2* json2 = zjsonInitWithString(jsonString);
//    
//    ZMap* map = json2->data;
//    
//    printf("%s \n", zmapGet(map, "name"));
//    
//    ZArray* array = zmapGet(map, "array");
//    
//    printf("%s \n", zarrayGet(array, 0));
//    
//    printf("%d \n", (1==3));
//}
//
//
//
//
//
//
//
//
//
//


//    main string str, list l, map m - string str
//    print "xxx"
//    l = [1, 3, 4, 5];
//    for i1, i2 in l print %d,%d,i1,i2
//
//    m = {"k1":"v1", "k2":"v2"};
//    l2 = m.keys
//    for k in l2 print m[k]
//
//    ret











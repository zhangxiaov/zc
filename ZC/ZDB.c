//
//  ZDB.c
//  ZC
//
//  Created by 张新伟 on 16/1/12.
//  Copyright © 2016年 张新伟. All rights reserved.
//

#include "ZDB.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "ZMap.h"
#include "ZJson.h"
#include <string.h>
#include <sys/malloc.h>
#include "ZTime.h"

static int globalPos = 0;

typedef struct _zrecord zrecord;
typedef struct _zdb zdb;

struct _zdb {
    zptr mmap;
};

struct _zrecord {
    char jsonStr[1024*2];
};

zptr zfileToMmap(char* fileName) {
    int fd = open(fileName, O_RDWR);
    struct stat st;
    fstat(fd, &st);
    zptr addr = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    return addr;
}

void zfileCreateEmptySpecifySize(char* fileName, int size) {
    int ret = truncate(fileName, size);
    if (ret != 0) {
        printf("failed when truncate\n");
    }
}

int zdbAdd(zdb* self, char* val) {
    int s = recordSize;
    if (strlen(val) > s) {
        printf("over recordSize\n");
        return 0;
    }
    
    memcpy(self->mmap+globalPos, val, strlen(val));
    
    zptr l = self->mmap + globalPos;
    int i = l - self->mmap;
    globalPos += recordSize;
    return globalPos-recordSize;
}

void zdbUpdate(zdb* self, int pos, char* val) {
    int s = recordSize;
    if (strlen(val) > s) {
        printf("over recordSize\n");
        return ;
    }
    memcpy(self->mmap+pos, val, strlen(val));
}

ZJson* zdbReadToJson(zdb* self, int pos) {
    int s = recordSize;
    char* jsonStr = (char*)malloc(s);
    memcpy(jsonStr, self->mmap+pos, s);
    ZJson* json = zjsonInitWithString(jsonStr);
    return json;
}

char* zdbReadToString(zdb* self, int pos) {
    int s = recordSize;
    char* jsonStr = (char*)malloc(s);
    memcpy(jsonStr, self->mmap+pos, s);
    return jsonStr;
}

zdb* zdbInit(char* fileName) {
    zdb* db = (zdb*)malloc(sizeof(zdb));
    zptr p = zfileToMmap(fileName);
    db->mmap = p;
    
    return db;
}

void zdbClose(zdb* self, int size) {
    munmap(self->mmap, size);
}

bool zdbCreate(char* fileName, int size) {
    int fd = open(fileName, O_RDWR);
    if (fd < 0) {
        printf("create new db file failed, desc:%s\n", strerror(errno));
        return false;
    }
    
    int ret = truncate(fileName, size);
    if (ret < 0) {
        printf("truncate new db file failed, desc:%s\n", strerror(errno));
        return false;
    }
    
    close(fd);
    return true;
}

void main_zdb() {

//void main() {
    char* dbName = "/tmp/test";
    int dbSize = 100000000;
    if (!zdbCreate(dbName, 100000000)) {
        return;
    }
    
    zdb* db = zdbInit(dbName);
    ZMap* map = zmapInit();
    zmapPut(map, "userName", "张新伟");
    zmapPut(map, "password", "123456");
    
    long s = ztimeSince1970();
    for (int i = 0; i < 30; i++) {
        int pos = zdbAdd(db, zmapToString(map));
        
        zmapPut(map, "age", "26");
        zdbUpdate(db, pos, zmapToString(map));
        
        //2
        int pos2 = zdbAdd(db, zmapToString(map));
        
        zmapPut(map, "age", "26");
        zdbUpdate(db, pos2, zmapToString(map));

    }
    
    printf("time = %d\n", ztimeSince1970() - s);
    
    int pos = zdbAdd(db, zmapToString(map));
    printf("pos = %d,\n content = %s\n", pos, zdbReadToString(db, pos));
    
    zmapPut(map, "age", "26");
    zdbUpdate(db, pos, zmapToString(map));
    printf("pos = %d,\n content = %s\n", pos, zdbReadToString(db, pos));
    
    //2
    int pos2 = zdbAdd(db, zmapToString(map));
    printf("pos = %d,\n content = %s\n", pos2, zdbReadToString(db, pos2));
    
    zmapPut(map, "age", "26");
    zdbUpdate(db, pos2, zmapToString(map));
    printf("pos = %d,\n content = %s\n", pos2, zdbReadToString(db, pos2));

    zdbClose(db, dbSize);
}




















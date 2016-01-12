#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
int main_test(int argc,char *argv[]){
    int fd,len;
    char *ptr;
//    if(argc<2){
//        printf("please enter a file\n");
//        return 0;
//    }
    if((fd=open("/tmp/zlog.log",O_RDWR))<0){
        perror("open file error");
        return -1;
    }
    len=lseek(fd,0,SEEK_END);
    
    
//    printf("file len := %d\n", len);
    char *s = "zadlfjasdlfjasdlfjasd";
    int len_s = strlen(s);
    
    
    ptr=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//读写得和open函数的标志相一致，否则会报错
    printf("%d\n", ptr);
    if(ptr==MAP_FAILED){
        perror("mmap error");
        close(fd);
        return -1;
    }
    close(fd);//关闭文件也ok
    printf("length is %ld\n",strlen(ptr));
    
    for (int i = 0; i < len/len_s; i++) {
        memcpy(ptr, s, strlen(s));
        ptr += len_s;
    }
    
    printf("%d\n", ptr);
    
    int totalLen = 10000 * len_s;
    munmap(ptr,totalLen);//将改变的文件写入内存
    return 0;
}
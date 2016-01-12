//
//  select_demo.c
//  ZC
//
//  Created by 张新伟 on 16/1/7.
//  Copyright © 2016年 张新伟. All rights reserved.
//

#include "select_demo.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/time.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>

int max_fd(int a[], int n)
{
    int max = 0;
    for(int i = 0; i < n; i++)
    {
        if(max < a[i])
        {
            max = a[i];
        }
    }
    
    return  max;
}

int main_select_demo(int argc, char*argv[])
{
    int port = 0;
    int N = 0;
    if (argc != 3)
    {
        printf("command error\n");
        exit(-1);
    }
    
    port = atoi(argv[1]);
    N = atoi(argv[2]);
    if(N > FD_SETSIZE)
    {
        N =   FD_SETSIZE;
    }
    int server_sock = 0;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    
    if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error\n");
        exit(-1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    
    if(bind(server_sock, (struct sockaddr*)&server_addr,sizeof(struct sockaddr)) == -1)
    {
        printf("bind error\n");
        exit(-1);
    }
    
    if(listen(server_sock, 5) == -1)
    {
        printf("listent error\n");
        exit(-1);
    }
    
    fd_set fd[2];
    FD_ZERO(&fd[0]);
    FD_SET(server_sock, &fd[0]);
    int* sock = malloc(sizeof(int*));
    memset(sock, 0, sizeof(int)*N);
    sock[0] = server_sock;
    int count = 0;
    
    while(1)
    {
        struct timeval tv = {5, 0};
        FD_ZERO(&fd[1]);
        fd[1] = fd[0];
        int ret = select(max_fd(sock, N)+1, &fd[1], NULL, NULL, &tv);
        if(ret < 0)
        {
//            cout<<"select error"<<endl;
            printf("select error\n");
        }
        else if(ret == 0)
        {
//            cout<<"time out"<<endl;
            printf("time out\n");
        }
        else
        {
            if(FD_ISSET(sock[0], &fd[1]) && count < N-1)
            {
                struct sockaddr_in client_addr;
                memset(&client_addr, 0, sizeof(client_addr));
                unsigned int len = sizeof(client_addr);
                int new_sock=accept(sock[0],(struct sockaddr*)&client_addr, &len);
                if(new_sock == -1)
                {
//                    cout<<"accept error"<<endl;
                    printf("accept error\n");
                }
                else
                {
                    for(int i = 1; i < N; i++)
                    {
                        if(sock[i] == 0)
                        {
                            sock[i] = new_sock;
                            FD_SET(new_sock, &fd[0]);
                            count++;
                            break;
                        }
                    }
                }
                
            }
            
            char recvbuf[1024] = {0};
            char sendbuf[1024] = {0};
            for(int i = 1; i < N; i++)
            {
                if(FD_ISSET(sock[i], &fd[1]))
                {
                    if(recv(sock[i], recvbuf, sizeof(recvbuf), 0) <= 0)
                    {
//                        cout<<"recv error"<<endl;
                        printf("recv error\n");
                        FD_CLR(sock[i], &fd[0]);
                        close(sock[i]);
                        sock[i] = 0;
                        count--;
                        continue;
                    }
                    
                    strcpy(sendbuf, recvbuf);
                    
                    if(send(sock[i], sendbuf, sizeof(sendbuf), 0) <= 0)
                    {
//                        cout<<"send error"<<endl;
                        printf("send error\n");
                        FD_CLR(sock[i], &fd[0]);
                        close(sock[i]);  
                        sock[i] = 0;  
                        count--;  
                        continue;  
                    }  
                }  
                
            }//end for   
            
        }  
        
    }//end while  
    return 0;  
    
}

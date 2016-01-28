#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>

//#pragma comment(lib,"ws2_32.lib")

// Function prototype
void StreamClient(char *szServer, short nPort, char *szMessage);
// Helper macro for displaying errors
#define PRINTERROR(s) \
fprintf(stderr,"\n%: %d\n", s, WSAGetLastError())



////////////////////////////////////////////////////////////



int main(int argc, char **argv)
{
    char hostname[255];
    
    WORD wVersionRequested = MAKEWORD(2,0);
    WSADATA wsaData;
    int nRet;
    short nPort;
    
    nPort =25;
    nRet = WSAStartup(wVersionRequested, &wsaData);
    
    StreamClient("smtp.126.com", nPort, "AUTH LOGIN\r\n");
    
    
    WSACleanup();
    return 0;
}

////////////////////////////////////////////////////////////

void StreamClient(char *szServer, short nPort, char *szMessage)
{
    printf("\nStream Client connecting to server: %s on port: %d",szServer, nPort);
    
    //
    // Find the server
    //
    LPHOSTENT lpHostEntry;
    
    lpHostEntry = gethostbyname(szServer);
    if (lpHostEntry == NULL)
    {
        PRINTERROR("gethostbyname()");
        return;
    }
    
    //
    // Create a TCP/IP stream socket
    //
    SOCKET theSocket;
    
    theSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (theSocket == INVALID_SOCKET){
        PRINTERROR("socket()");
        return;
    }
    
    //
    // Fill in the address structure
    //
    SOCKADDR_IN saServer;
    
    saServer.sin_family = AF_INET;
    saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
    saServer.sin_port = htons(nPort);
    
    //
    // connect to the server
    //
    int nRet;
    
    nRet = connect( theSocket, // Socket
                   (LPSOCKADDR)&saServer, // Server address
                   sizeof(struct sockaddr));// Length of server address structure
    printf("\n%d1lL=====================================%d\n",nRet,SOCKET_ERROR);
    if (nRet == SOCKET_ERROR)
    {
        PRINTERROR("socket()");
        closesocket(theSocket);
        return;
    }
    
    
    char szBuf[256];
    
    
    // strcpy(szBuf, "From the Client");
    char buf[350]="0";
    char server[250];
    gethostname(server,250);
    strcpy(buf, "HELO ");
    
    strcat(buf, server);
    strcat(buf, " \r\n");
    printf("%s:::%d",buf,strlen(buf));
    
    //
    // Wait for a reply
    //
    nRet = recv(theSocket,szBuf,sizeof(szBuf)+1,0);  printf("\nData received OVER DATA: %s", szBuf);
    
    nRet = send(theSocket, buf, strlen(buf), 0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf), 0); printf("\nData received2: %s", szBuf);
    
    //发送准备登陆信息
    nRet = send(theSocket, "AUTH LOGIN \r\n", sizeof("AUTH LOGIN \r\n"), 0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf), 0); printf("\nData received LOGIN: %s", szBuf);
    
    //发送用户名和密码,这里的用户名和密码必须用base64进行转码,发送转码以后的字符串
    nRet = send(theSocket,"用户名\r\n", strlen("用户名\r\n"),0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf),0);printf("\nData receivedUSERNAME: %s", szBuf);
    
    //发送用户密码
    nRet = send(theSocket,"密码\r\n", strlen("密码\r\n"), 0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf),0); printf("\nData receivedUSERPASSWORD: %s", szBuf);
    
    //发送[发送邮件]的信箱(改成你的邮箱!)
    send(theSocket,"MAIL FROM:<geniues@126.com>\r\n",sizeof("MAIL FROM:<geniues@126.com>\r\n"),0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf)+1, 0);printf("\nData received MAILFROM: %s", szBuf);
    
    //发送[接收邮件]的邮箱
    nRet= send(theSocket,"RCPT TO:<geniues@qq.com> \r\n",sizeof("RCPT TO:<geniues@qq.com> \r\n"),0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf)+1, 0);   printf("\nData received TOMAIL: %s", szBuf);
    
    char MailData[1024] =
    "From: \"geniues@126.com\"\r\n"
    "To: maxinliang002@126.com\r\n"
    "Subject: IP Address\r\n\r\n"; //主题可以改成别的
    
    //各诉邮件服务器，准备发送邮件内容
    send(theSocket,"DATA\r\n", strlen("DATA\r\n"),0);
    nRet = recv(theSocket, szBuf, sizeof(szBuf)+1, 0); printf("\nData receivedSEND DATA: %s", szBuf);
    //发送邮件标题
    send(theSocket,MailData, strlen(MailData),0);
    //发送邮件内容
    send(theSocket,"邮件内容来了!\r\n", strlen("邮件内容来了!\r\n"),0);
    //发送邮件结束
    send(theSocket,"\r\n.\r\n", strlen("\r\n.\r\n"),0);
    //接收邮件服务器返回信息
    nRet = recv(theSocket,szBuf,sizeof(szBuf)+1,0);  printf("\nData received OVER DATA: %s", szBuf);
    
    
    
    // 
    // Display the received data 
    // 
    //printf("\nData received3: %s", szBuf); 
    
    
    closesocket(theSocket); 
    return; 
}
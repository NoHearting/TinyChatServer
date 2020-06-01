/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 15:25:09
 * @LastEditors: zsj
 * @LastEditTime: 2020-05-31 21:52:01
 */ 

#include "ChatConnection.h"

void ChatConnection::process()
{
    cout<<pthread_self()<<" get a task("<<fd_<<") and process ";
    char buffer[256];
    bzero(buffer,sizeof(buffer));
    int len = recv(fd_,buffer,256,0);
    if(len <= 0){
        printf("recv error!\n");
    }
    else{
        printf(" message is %s \n",buffer);
    }
}
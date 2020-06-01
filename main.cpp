/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-30 20:38:14
 * @LastEditors: zsj
 * @LastEditTime: 2020-05-31 20:58:34
 */ 
#include<event.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<iostream>
#include<signal.h>
#include<map>
#include<set>

#include"utils/utils.h"
#include"./chatserver/ChatServer.h"

using namespace std;


int main(int argc,char * argv[])
{
    auto server = ChatServer::getInstance();

    //线程池
    server->initThreadPool();

    //监听
    server->eventListen();

    //开启事件循环
    server->eventLoop();
    
}
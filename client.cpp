/*
 * @Descripttion: 
 * @version: 
 * @Author: zsj
 * @Date: 2020-06-01 20:34:47
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-01 20:45:31
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
#include<string>
#include<iostream>

using namespace std;


int main(int argc,char * argv[])
{
    const char * ip = "127.0.0.1";
    int port = 9999;
    int clnt_sock = socket(PF_INET,SOCK_STREAM,0);
    sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);


    int serv_sock = connect(clnt_sock,(sockaddr*)&serv_addr,sizeof(serv_addr));

    while(true)
    {
        string buffer;
        cin>>buffer;
        int ret = write(serv_sock,buffer.c_str(),buffer.size());

    }
    close(serv_sock);
    close(clnt_sock);
}
/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 15:24:45
 * @LastEditors: zsj
 * @LastEditTime: 2020-05-31 21:46:06
 */ 
#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>

#include<string.h>
#include<sys/types.h>
#include<sys/syscall.h>
using namespace std;

class ChatConnection
{
public:
    void process();
    ChatConnection(int fd):fd_(fd){}
    ChatConnection():fd_(-1){}
private:
    int fd_;
};

#endif
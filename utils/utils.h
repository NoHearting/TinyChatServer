/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-30 20:51:40
 * @LastEditors: zsj
 * @LastEditTime: 2020-05-31 18:30:10
 */ 
#ifndef UTILS_H
#define UTILS_H

#include<stdio.h>
#include<event.h>
#include<time.h>
#include<signal.h>
#include<unordered_map>
#include<vector>
#include<memory>
#include<sys/socket.h>

#include"../chatserver/ChatServer.h"


#define LOG(format,...)  {printf("[%s,%d] ",__FILE__,__LINE__);printf(format,##__VA_ARGS__);}

using namespace std;

class Utils
{
public:
    
    Utils(vector<shared_ptr<struct event>> & events):events_(events){}
private:
    vector<shared_ptr<struct event>> & events_;
};

/**
 * @brief 信号处理器的回调函数
 */ 
void signalCallback(int fd,short event,void * arg);

/**
 * @brief 定时事件处理器的回调函数
 */ 
void timerCallback(int fd,short event,void * arg);

/**
 * @brief I/O事件处理器的回调函数,处理远程连接请求
 */ 
void socketLinkCallback(int fd,short event,void * arg);


/**
 * @brief I/O事件处理器的回调函数,处理数据传输
 */ 
void socketTransmitCallback(int fd,short event,void * arg);



#endif
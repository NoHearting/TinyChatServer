/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-30 20:51:40
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-01 23:21:09
 */ 
#ifndef UTILS_H
#define UTILS_H

#include<stdio.h>
#include<event.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
#include<memory>
#include<sys/socket.h>


#define LOG(format,...)  {printf("[%s,%d] ",__FILE__,__LINE__);printf(format,##__VA_ARGS__);}

using namespace std;

class Utils
{
public:
    
    Utils(){}
private:
};



/**
 * @name: 
 * @brief: 文件描述服设置为非阻塞
 * @param {type} 
 * @return: 
 */
int setnonblocking(int fd);




#endif
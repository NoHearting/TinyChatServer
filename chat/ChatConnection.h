/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 15:24:45
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-02 15:56:51
 */ 
#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include<iostream>
#include<sys/socket.h>
#include<string.h>
#include<string.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<unordered_map>

// #include"../chatserver/ChatServer.h"
#include "../utils/libevent_utils.h"

using namespace std;

const int READ_BUFFER_SIZE = 1024;


class ChatConnection
{
public:
    
    ChatConnection(int fd,LibeventUtils * libeventUtils,unordered_map<int,ChatConnection> * users):
    fd_(fd),libeventUtils_(libeventUtils),readIndex_(0),users_(users){
        bzero(readBuffer_,0);
    }
    ChatConnection():
    fd_(-1),readIndex_(0),users_(nullptr){
        bzero(readBuffer_,0);
    }


    void process();

    void close();

    /**
     * @name read 
     * @brief 读取数据，判断是否可以读取
     * @param 
     * @return 
     */
    bool read();


    /**
     * @brief 发送数据，发送给客户端
     */ 
    bool write(int fd);

    void sendAllClient();

private:
    
    int fd_; //文件描述符


    //缓冲区
    char readBuffer_[READ_BUFFER_SIZE];   //消息缓冲区，读取socket发送的数据
    char readIndex_;    

    unordered_map<int,ChatConnection> * users_;

    //libevent管理
    LibeventUtils * libeventUtils_;

    void clearBuffer(){
        bzero(readBuffer_,READ_BUFFER_SIZE);
        readIndex_ = 0;
    }   
};

#endif
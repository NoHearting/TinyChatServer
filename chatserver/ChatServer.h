/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 15:21:27
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-02 17:34:06
 */ 
#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>
#include <event.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <algorithm>
#include <map>
#include <exception>


#include "../chat/ChatConnection.h"
#include "../threadpool/threadpool.h"
#include "../utils/libevent_utils.h"

using namespace std;

// class ChatConnection;

class ChatServer
{
public:
    

    ~ChatServer();

    static shared_ptr<ChatServer> getInstance()
    {
        return chatServer_;
    }

    void init(int port,int threadNum);

    /**
     * @brief 初始化线程池
     */ 
    void initThreadPool();


    /**
     * @brief 监听连接请求
     */ 
    void eventListen();

    /**
     * @brief 开启事件循环
     */ 
    void eventLoop();


    

    /**
     * @brief 添加新连接的用户
     */ 
    ChatConnection & addConnectionUser(int fd){
        users_[fd] = ChatConnection(fd,libeventUtils_,&users_);
        return users_[fd];
    }

    /**
     * @brief 添加一个任务到任务队列
     */ 
    void appendTask(ChatConnection & chatConnection){
        pool_->append(&chatConnection);
    }



    //getter and setter
    int getPort()const{return port_;}
    void setPort(int port){port_ = port;}

    int getThreadNum(){return threadNum_;}
    void setThreadNum(int threadNum){threadNum_ = threadNum;}

    unordered_map<int,ChatConnection>& getUsers(){return users_;}

    LibeventUtils * getLibeventUtils(){return libeventUtils_;}

    ChatConnection & getConnection(int fd){
        if(users_.find(fd) != users_.end()){
            return users_[fd];
        }
        else{
            throw exception();
        }
    }

    

    /**
     * @brief 关闭一个连接
     */ 
    void closeConnection(int fd){
        if(users_.find(fd) != users_.end()){
            event * ev = libeventUtils_->popEvent(fd);
            event_del(ev);
            event_free(ev);
            users_.erase(users_.find(fd));
            close(fd);
        }
        else{
            throw exception();
        }
    }


private:

    static shared_ptr<ChatServer> chatServer_;

    //基本
    int port_;  //服务器端口
    unordered_map<int,ChatConnection> users_;  //连接的用户

    //线程池相关
    shared_ptr<threadpool<ChatConnection>> pool_;  //线程池
    int threadNum_;  //线程数量

    //libevent相关
    LibeventUtils * libeventUtils_;



    ChatServer(int port = 9999,int threadNum = 8);

   

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
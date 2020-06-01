/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 15:21:27
 * @LastEditors: zsj
 * @LastEditTime: 2020-05-31 21:02:10
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


#include "../chat/ChatConnection.h"
#include "../threadpool/threadpool.h"
#include "../utils/utils.h"

using namespace std;

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
     * @brief 创建一个定时事件处理器并且添加到注册事件队列
     * @param cb 定时器回调函数
     * @param timeout 定时器延迟事件
     *
     */ 
    void addTimerEvent(event_callback_fn cb,const struct timeval *timeout);

    /**
     * @brief 创建一个信号事件处理器并添加到注册事件队列
     * @param 信号值
     * @param cb 回调函数
     */ 
    void addSignalEvent(evutil_socket_t fd, event_callback_fn cb);


    /**
     * @brief 创建一个I/O事件处理器并添加到注册事件队列
     * @param 文件描述符值
     * @param events 事件类型
     * @param cb 回调函数
     */ 
    void addSocketEvent(evutil_socket_t fd, short events, event_callback_fn cb);

    /**
     * @brief 添加新连接的用户
     */ 
    ChatConnection & addConnectionUser(int fd){
        users_[fd] = ChatConnection(fd);
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



private:

    static shared_ptr<ChatServer> chatServer_;

    //基本
    int port_;  //服务器端口
    unordered_map<int,ChatConnection> users_;  //连接的用户

    //线程池相关
    shared_ptr<threadpool<ChatConnection>> pool_;  //线程池
    int threadNum_;  //线程数量

    //libevent相关
    struct event_base * base_;
    vector<struct event *> events_;  //存放所有注册的事件处理器



    ChatServer(int port = 9999,int threadNum = 8);

   
    

    


    


};

#endif
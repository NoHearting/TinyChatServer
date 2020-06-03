/*
 * @Descripttion: 
 * @version: 
 * @Author: zsj
 * @Date: 2020-06-01 21:32:39
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-02 16:44:57
 */ 
#ifndef LIBEVENT_UTILS_H
#define LIBEVENT_UTILS_H

#include<event.h>
#include<iostream>
#include<map>
#include<signal.h>


#include"utils.h"

using namespace std;

class ChatServer;

class LibeventUtils
{

public:
    LibeventUtils():base_(event_init()){}
    explicit LibeventUtils(event_base * base):base_(base){}
    ~LibeventUtils();

    /**
     * @brief 开始循环
     */ 
    void dispatch(){event_base_dispatch(base_);}

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
     * @brief 获取一个事件处理器,并从容器中弹出
     */ 
    event * popEvent(int fd){
        if(events_.find(fd) != events_.end()){
            event * ev = events_[fd];
            events_.erase(events_.find(fd));  //从当前容器中移除
            cout<<"pop a event and remove it from container!"<<endl;
            return ev;
        }
        else{
            throw exception();
        }
    }
private:
    event_base * base_;
    map<int,event *> events_;

};




#endif
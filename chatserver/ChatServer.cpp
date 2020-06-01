/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 14:41:20
 * @LastEditors: zsj
 * @LastEditTime: 2020-05-31 21:59:22
 */ 
#include "ChatServer.h"

shared_ptr<ChatServer> ChatServer::chatServer_ = shared_ptr<ChatServer>(new ChatServer());


ChatServer::ChatServer(int port,int threadNum):port_(port),threadNum_(threadNum)
{
    printf("init server.....\n");
    base_ = event_init();
}

ChatServer::~ChatServer()
{
    for(int i = 0;i<events_.size();i++)
    {
        event_free(events_[i]);
    }
    event_base_free(base_);
}


void ChatServer::init(int port,int threadNum)
{
    port_ = port;
    threadNum_ = threadNum;
}

void ChatServer::initThreadPool()
{
    pool_ = shared_ptr<threadpool<ChatConnection>>(new threadpool<ChatConnection>(threadNum_));
}

void ChatServer::eventLoop()
{
    event_base_dispatch(base_);
}

void ChatServer::eventListen()
{
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1){
        LOG("socket() failed!!!");
        exit(-1);
    }

    //设置在time-wait状态的套接字可以继续分配
    int option = 1;
    int optlen = sizeof(option);
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(void *)&option,optlen);


    sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port_);

    int ret = bind(listenfd,(sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret == -1){
        LOG("bind() failed !!!\n");
        exit(-1);
    }

    ret = listen(listenfd,5);
    if(ret == -1){
        LOG("listen() failed !!!!\n");
        exit(-1);
    }

    //将当前监听socket添加到事件队列中
    addSocketEvent(listenfd,EV_READ|EV_PERSIST,socketLinkCallback);

    addSignalEvent(SIGPIPE,NULL);

    printf("add SIGALEM signal\n");
    addSignalEvent(SIGALRM,signalCallback);

    printf("add SIGTERM signal\n");
    addSignalEvent(SIGTERM,signalCallback);

    printf("add SIGINT signal\n");
    addSignalEvent(SIGINT,signalCallback);

}


void ChatServer::addSocketEvent(evutil_socket_t fd, short events, event_callback_fn cb)
{
    event* socketEvent = event_new(base_,fd,events,cb,base_);
    event_add(socketEvent,NULL);
    events_.push_back(socketEvent);
}

void ChatServer::addSignalEvent(evutil_socket_t fd, event_callback_fn cb)
{
    event* signalEvent = evsignal_new(base_,fd,cb,base_);
    event_add(signalEvent,NULL);
    events_.push_back(signalEvent);
}

void ChatServer::addTimerEvent(event_callback_fn cb,const struct timeval *timeout)
{

}






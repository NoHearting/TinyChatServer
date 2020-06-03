/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 14:41:20
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-02 16:01:05
 */ 
#include "ChatServer.h"

shared_ptr<ChatServer> ChatServer::chatServer_ = shared_ptr<ChatServer>(new ChatServer());


ChatServer::ChatServer(int port,int threadNum):port_(port),threadNum_(threadNum)
{
    printf("init server.....\n");
    libeventUtils_ = new LibeventUtils();
}

ChatServer::~ChatServer()
{
    delete libeventUtils_;
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
    libeventUtils_->dispatch();
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
    libeventUtils_->addSocketEvent(listenfd,EV_READ|EV_PERSIST,socketLinkCallback);

    libeventUtils_->addSignalEvent(SIGPIPE,NULL);

    printf("add SIGALEM signal\n");
    libeventUtils_->addSignalEvent(SIGALRM,signalCallback);

    printf("add SIGTERM signal\n");
    libeventUtils_->addSignalEvent(SIGTERM,signalCallback);

    printf("add SIGINT signal\n");
    libeventUtils_->addSignalEvent(SIGINT,signalCallback);

}






// Callback
void signalCallback(int fd,short event,void * arg)
{
    printf("signal value is %d\n",fd);
    switch(fd)
    {
        case SIGALRM:  //定时信号  14
        {
            printf("time tick!!!");
            break;
        }
        case SIGTERM:  
        {
            break;
        }
        case SIGINT: //中断信号  2
        {
            struct event_base * base = static_cast<event_base *>(arg);
            struct timeval delay = {2,0};
            printf("Caught an interrupt signal;exiting cleanly in 2 seconds...\n");
            event_base_loopexit(base,&delay);
            break;
        }
    }
}

void timerCallback(int fd,short event,void * arg)
{

}

void socketLinkCallback(int fd,short event,void * arg)
{
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    int connfd = accept(fd,(sockaddr*)&clnt_addr,&clnt_addr_sz);
    if(connfd < 0){
        printf("Client line the server failed! errno is: %d\n",errno);
        return;
    }
    printf("get a connection: %d\n",connfd);

    setnonblocking(connfd);

    ChatServer::getInstance()->getLibeventUtils()->addSocketEvent(connfd,EV_READ|EV_PERSIST,socketTransmitCallback);
    ChatServer::getInstance()->addConnectionUser(connfd);
}

void socketTransmitCallback(int fd,short event,void * arg)
{
    auto & user = ChatServer::getInstance()->getConnection(fd);
    if(user.read()){
        cout<<"add a task to queue!"<<endl;
        ChatServer::getInstance()->appendTask(user);
    }
    else{
        cout<<"one client disconnecting..."<<endl;
        ChatServer::getInstance()->closeConnection(fd);
    }
}



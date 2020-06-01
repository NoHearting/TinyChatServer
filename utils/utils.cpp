#include "utils.h"


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
    ChatServer::getInstance()->addSocketEvent(connfd,EV_READ|EV_PERSIST,socketTransmitCallback);
}

void socketTransmitCallback(int fd,short event,void * arg)
{
    auto & user = ChatServer::getInstance()->addConnectionUser(fd);
    ChatServer::getInstance()->appendTask(user);
}
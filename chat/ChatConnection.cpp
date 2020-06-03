/*
 * @Descripttion: 
 * @Version: 
 * @Author: zsj
 * @Date: 2020-05-31 15:25:09
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-02 16:50:52
 */ 

#include "ChatConnection.h"

void ChatConnection::process()
{
    //此处对数据进行封装，消息编码
    // ....

    //暂时只输出
    cout<<pthread_self()<<" get a task("<<fd_<<") and process ";
    printf("  message is %s\n",readBuffer_);
    // clearBuffer();

    //将信息发送回所有客户端
    sendAllClient();
}

bool ChatConnection::read()
{
    if(readIndex_ > READ_BUFFER_SIZE){
        return false;
    }
    int bytes_read = 0;
    while(true)
    {
        bytes_read = recv(fd_,readBuffer_+readIndex_,READ_BUFFER_SIZE-readIndex_,0);
        if(bytes_read == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                LOG("recv -1 break\n");
                break;
            }
            LOG("recv -1\n");
            return false;

        }
        else if(bytes_read == 0)
        {
            LOG("recv 0\n");
            return false;
        }
        readIndex_ += bytes_read;
    }
    readBuffer_[readIndex_] = 0;
    LOG("recv ok\n");
    return true;
}


void ChatConnection::sendAllClient()
{
    // auto & users = ChatServer::getInstance()->getUsers();
    auto & users = *users_;
    for(auto iter = users.begin();iter != users.end();++iter)
    {
        if(!write(iter->first))
        {
            // ChatServer::getInstance()->closeConnection(iter->first);
            cout<<"send error from socket: "<<iter->first<<endl;
        }
        else{
            cout<<"send to "<<iter->first<<" message ok!"<<endl;
        }
    }
    clearBuffer();
}

bool ChatConnection::write(int fd)
{
    int bytesHaveSend = 0;
    int bytesToSend = readIndex_;
    int temp = 0;
    if(bytesToSend == 0){
        clearBuffer();
        return true;
    }
    while(true)
    {
        temp = send(fd,readBuffer_,readIndex_,0);
        if(temp <= -1)
        {
            if(errno == EAGAIN)
            {
                return true;
            }
            return false;
        }
        bytesToSend -= temp;
        bytesHaveSend += temp;
        if(bytesToSend <= bytesHaveSend)
        {
            return true;
        }

    }
}

void ChatConnection::close()
{
    cout<<"remote client was disconnected!!!"<<endl;
    // ChatServer::getInstance()->closeConnection(fd_);
}
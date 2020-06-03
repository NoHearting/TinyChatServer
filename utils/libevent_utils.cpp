/*
 * @Descripttion: 
 * @version: 
 * @Author: zsj
 * @Date: 2020-06-01 21:41:23
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-01 21:47:54
 */ 

#include "libevent_utils.h"

LibeventUtils::~LibeventUtils()
{
    for(auto iter = events_.begin();iter != events_.end();++iter)
    {
        event_free(iter->second);
    }
    event_base_free(base_);

    
}



void LibeventUtils::addSocketEvent(evutil_socket_t fd, short events, event_callback_fn cb)
{
    event* socketEvent = event_new(base_,fd,events,cb,base_);
    event_add(socketEvent,NULL);
    events_.insert(pair<int,event*>(fd,socketEvent));
}

void LibeventUtils::addSignalEvent(evutil_socket_t fd, event_callback_fn cb)
{
    event* signalEvent = evsignal_new(base_,fd,cb,base_);
    event_add(signalEvent,NULL);
    events_.insert(pair<int,event*>(fd,signalEvent));
}

void LibeventUtils::addTimerEvent(event_callback_fn cb,const struct timeval *timeout)
{
    
}


/*
 * @Descripttion: 
 * @version: 
 * @Author: zsj
 * @Date: 2020-06-01 14:26:48
 * @LastEditors: zsj
 * @LastEditTime: 2020-06-01 23:16:08
 */ 
#include "utils.h"

//对文件描述符设置非阻塞
int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}



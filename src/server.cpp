/*
 * =====================================================================================
 *
 *     Filename:  multiprocesslog.cpp
 *
 *    Description:  
 *
 *      Version:  1.0
 *       Created:  11/15/2011 02:48:44 PM
 *       Revision:  none
 *        Compiler:  gcc
 *
 *        Author:  chenquan 
 *        Company:  
 *
 * =====================================================================================
 */

#include <log4cplus/fileappender.h>
#include <log4cplus/socketappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <time.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/helpers/socket.h>
#include <stdio.h>
#include <string>

using namespace log4cplus;
using namespace std;
using namespace log4cplus::helpers;

//server端
int log_server(int port)
{
    //创建一个socket，绑定端口
    helpers::ServerSocket serverSocket(port);

    PropertyConfigurator::doConfigure("./log4c_server.conf");
    Logger logger = Logger::getInstance("Client");


    while(1)
    {
        //accept
        helpers::Socket clientsock = serverSocket.accept();
        if ( clientsock.isOpen() == false )
        {
            perror("accept err.");
            continue;
        }
    
        //没个链接都fork一个进程 待优化
        if (!fork()){ //子进程
            while(1){
                //每次都重新重新申请内存，待优化
                SocketBuffer msgSizeBuffer(sizeof(unsigned int));
                if(!clientsock.read(msgSizeBuffer))
                {
                    printf("read size err");
                    exit(0);
                }
                unsigned int msgSize = msgSizeBuffer.readInt();
                SocketBuffer buffer(msgSize);
                //读取日志消息到buffer
                if(!clientsock.read(buffer))
                {
                    printf("read buff err");
                    exit(0);
                }

                //转化成event
                spi::InternalLoggingEvent event = readFromBuffer(buffer);
                int level = event.getLogLevel();
                logger.callAppenders(event);
            }
        }
    }

    return 0;
}

int main()
{
    log_server(7838);
    return 0;
}

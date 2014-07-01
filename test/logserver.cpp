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
 *        Author:  Xu Zhe Ming, 
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

    PropertyConfigurator::doConfigure("../log4c_client.conf");
    Logger logger = Logger::getInstance("Client");


    while(1)
    {
        //accept
        helpers::Socket clientsock = serverSocket.accept();
        SocketBuffer msgSizeBuffer(sizeof(unsigned int));
        if(!clientsock.read(msgSizeBuffer))
        {
            return 0;
        }
        unsigned int msgSize = msgSizeBuffer.readInt();
        SocketBuffer buffer(msgSize);
        //读取日志消息到buffer
        if(!clientsock.read(buffer))
        {
            return 0;
        }

        //转化成event
        spi::InternalLoggingEvent event = readFromBuffer(buffer);
        int level = event.getLogLevel();
        logger.callAppenders(event);
    }

    return 0;
}

//client端
int log_client(string server_ip, int server_port)
{
    SharedAppenderPtr _append(new SocketAppender(server_ip, server_port, "ServerName"));
    Logger::getRoot().addAppender(_append);
    while(1)
    {
        int rand_num = rand();
        int remainder = rand_num % 2;
        if (remainder == 0)
            LOG4CPLUS_INFO(Logger::getRoot(), "client logging event number ... ");
        else
            LOG4CPLUS_DEBUG(Logger::getRoot(), "client logging odd number... ");
        //sleep(1);
    }
}


int main()
{

    pid_t pid = fork();

    //child, let's run a log server process
    if(pid == 0)
    {
        log_server(20000);
    }
    //parent, let's fork some more processes to write logs
    else
    {
        for(int i = 0; i < 5; i++)
        {
            pid = fork();
            //child, let's run a log client process
            if(pid == 0)
            {
                log_client("localhost", 20000);
            }
        }
    }

    while(1)
    {
        //sleep(1);
    }

    return 0;
}

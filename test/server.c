#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MAXDATASIZE 1024

int main(int argc, char ** argv)
{
    int sockfd,new_fd,numbytes; /* 监听socket: sock_fd,数据传输socket: new_fd */
    struct sockaddr_in my_addr; /* 本机地址信息 */
    struct sockaddr_in their_addr; /* 客户地址信息 */
    unsigned int sin_size, myport, lisnum;
    char buf[MAXDATASIZE];
    int firstPacket = 1, recvLen = 4;

    if(argv[1])
        myport = atoi(argv[1]);
    else 
        myport = 7838;

    if(argv[2])  
        lisnum = atoi(argv[2]);
    else 
        lisnum = 2;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket"); 
        exit(1); 
    }

    my_addr.sin_family=PF_INET;
    my_addr.sin_port=htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 0);
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, lisnum) == -1) {
        perror("listen");
        exit(1);
    }

    while(1) {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        
        if (!fork()) { /* 子进程代码段 */
            firstPacket = 1;
            recvLen = 4;
            while(1){
                memset(buf, 0, MAXDATASIZE);
                numbytes=recv(new_fd, buf, recvLen, 0);
                if( numbytes == 0 ){
                    perror("client close socket");
                    close(new_fd);
                    exit(0);
                }
                else if( numbytes > 0){
                    if(firstPacket == 1 && numbytes == 4){
                            printf("recv firstPacket len: %s\n", buf);
                            firstPacket == 0;
                            recvLen = atoi(buf);
                    }
                    else if(firstPacket == 1 && numbytes == recvLen){
                            firstPacket == 1;
                            recvLen = 4;
                    }
                    else if(firstPacket == 1 && numbytes < recvLen){
                            recvLen -= numbytes;
                    }
                    else{
                            perror("recv err~");
                    }
                
                }
                else{
                    perror("recv error");
                    exit(1);
                }
            }
        }
        close(new_fd); /*父进程不再需要该socket*/
        waitpid(-1,NULL,WNOHANG);/*等待子进程结束，清除子进程所占用资源*/
    }
}


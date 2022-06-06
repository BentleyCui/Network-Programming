#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "wrap.h"

#define SERV_PORT 9527

int main(){
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1){
        perror("create socket error");
        exit(1);
    }

    struct sockaddr_in serv_addr;    //服务器地址结构 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    int ret = connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    int counter = 10;
    char buf[BUFSIZ];
    while(--counter){

        write(cfd, "hello\n",6);
        
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        sleep(1);
    }

    close(cfd);

    return 0;
}



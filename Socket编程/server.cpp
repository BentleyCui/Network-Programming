#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "wrap.h"

#define SERV_PORT 9527


int main(){
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("create socket error");
        exit(1);
    }

    struct sockaddr_in sev_addr;
    sev_addr.sin_family = AF_INET;
    sev_addr.sin_port = htons(SERV_PORT);
    sev_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(lfd, (struct sockaddr*)&sev_addr, sizeof(sev_addr));
    
    listen(lfd, 128);

    struct sockaddr_in clit_addr;
    socklen_t clit_addr_len = sizeof(clit_addr);
    int cfd=0;
    cfd = accept(lfd, (struct sockaddr*)&clit_addr, &clit_addr_len);
    if(cfd == -1){
        perror("create socket error");
        exit(1);
    }

    char client_IP[1024];
    printf("client ip: %s  port: %d\n", 
        inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)),
        ntohs(clit_addr.sin_port));

    int ret;
    char buf[BUFSIZ];
    while(1){
        ret = read(cfd, buf, sizeof(buf));

        write(STDOUT_FILENO, buf, ret); //打印查看

        for(int i=0; i<ret; i++){
            buf[i] = toupper(buf[i]);
        }

        write(cfd, buf, ret);
    }

    close(lfd);
    close(cfd);

    return 0;
}


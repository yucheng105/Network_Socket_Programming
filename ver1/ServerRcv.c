#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>





void *ServerRcv(void *arg) {

    int fd = *(int *)arg;
    char buf[MAXDATASIZE];
    int n;

    // recv nonstop
    while (1) {

        if (recv(fd, buf, sizeof(buf), 0) > 0)  {
            // 成功收到
            buf[n] = '\0';
            printf("%s", buf);
        }
        
    }
    
    return NULL;
}
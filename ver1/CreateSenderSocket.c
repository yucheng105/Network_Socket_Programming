#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>


int CreateSenderSocket(char* hostname, char* port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int status;
    char buffer[MAXDATASIZE] = {0};
    
    /*-------Hints--------*/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    /*-------getaddrinfo()-------*/
    if ((status = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    
    
    // go through all nodes
    for (p = servinfo; p != NULL; p = p->ai_next) {
        /*----------Create Socket-----------*/
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            continue;
        }
        
        // TODO: Bind
        
        /*-------connect()-------*/
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("connect");
            continue;
        }
        break; 
    }
    
    
    if (p == NULL) {
        fprintf(stderr, "failed to connect\n");
        return -1;
    }
    
    // connection successful
    printf("Connection Successful\r\n");
    
    // print additional info regarding the host
    char s[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, 
              p->ai_family == AF_INET ? 
                  (void*)&((struct sockaddr_in*)p->ai_addr)->sin_addr :
                  (void*)&((struct sockaddr_in6*)p->ai_addr)->sin6_addr,
              s, sizeof s);
    printf("client: connecting to %s\r\n\n", s);
    
    freeaddrinfo(servinfo);
    
    return sockfd;
}



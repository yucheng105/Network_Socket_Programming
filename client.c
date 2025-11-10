#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>  

#define MAXDATASIZE 1024  
int main(int argc, char *argv[]) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int status;
    char buffer[MAXDATASIZE] = {0};
    
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        exit(1);
    }
    
    /*-------Hints--------*/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    /*-------getaddrinfo()-------*/
    if ((status = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    
    
    for (p = servinfo; p != NULL; p = p->ai_next) {
        /*----------Create Socket-----------*/
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            continue;
        }
        
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
        return 2;
    }
    
    // connection successful
    printf("Connection Successful\r\n");
    
    
    
    
    
    

    char s[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, 
              p->ai_family == AF_INET ? 
                  (void*)&((struct sockaddr_in*)p->ai_addr)->sin_addr :
                  (void*)&((struct sockaddr_in6*)p->ai_addr)->sin6_addr,
              s, sizeof s);
    printf("client: connecting to %s\n", s);
    
    freeaddrinfo(servinfo);
    
    
    
    
    
    
    
/*
    const char *hello = "Hello from client";
    if (send(sockfd, hello, strlen(hello), 0) == -1) {
        perror("send");
        close(sockfd);
        return 1;
    }
    printf("Hello message sent\n");
    

    int numbytes = recv(sockfd, buffer, MAXDATASIZE-1, 0);
    if (numbytes == -1) {
        perror("recv");
        close(sockfd);
        return 1;
    }
    
    buffer[numbytes] = '\0';
    printf("client: received '%s'\n", buffer);
    
*/
    
    close(sockfd);
    return 0;
}
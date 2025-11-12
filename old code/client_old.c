#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>  

#define MAXDATASIZE 1024  


// return socket file descriptor
int CreateClientSocket(char* hostname, char* port);

void CommunicateWithServer(int sockfd);


int main(int argc, char *argv[]) {      // IP PORT
    int sockfd;     

    
    if (argc != 3) {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        exit(1);
    }
    
    sockfd = CreateClientSocket(argv[1], argv[2]);

    CommunicateWithServer(sockfd);
    
    
    
    
    

    // close socket
    close(sockfd);
    
    
    
    return 0;
}


int CreateClientSocket(char* hostname, char* port)
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




void CommunicateWithServer(int serverSocketFd)  {
    
    int sockfd = serverSocketFd;
    
    
    // to buffer input cmds
    char clientInput[MAXDATASIZE] = {0};
    // buffer server response
    char serverResponse[MAXDATASIZE] = {0};
    
    
    while (1)   {
        
        // init buffers
        memset(clientInput, 0, sizeof(clientInput));
        memset(serverResponse, 0, sizeof(serverResponse));
        


        // INPUT
        fgets(clientInput, sizeof(clientInput), stdin);
        // extremely important
        clientInput[strcspn(clientInput, "\n")] = '\0';
        
        // if user presses enter, ignore
        if (strlen(clientInput) == 0)
            continue;
        
        
        //////////////////////////////////////////////////////////////////////
        // Check clientInput: return the proper socket descriptor to send to
        //////////////////////////////////////////////////////////////////////
        
        // check if is p2p payment, if yes, send to peer
        int hashCnt = 0;
        for (i = 0; i < strlen(clientInput); i++)   {
            if (clientInput[i] == '#')
                hashCnt++;
        }
        // send2peers: p2p PAYMENT msg has 2 '#'
        if (hashCnt == 2)   {
            // update sockfd to peer socket

        }
        // send2server
        else    {
            sockfd = serverSocketFd;
        }
        
        
        ///////////////////////
        // SEND AND RECV MSG
        ///////////////////////
        
        // send msg to server
        if (send(sockfd, clientInput, strlen(clientInput), 0) == -1) {
            perror("send");
            continue;
        }
        else    {
            printf("Awaiting Server Response...\r\n");
        }
        
        // recieving msg from server
        int recvStatus = recv(sockfd, serverResponse, sizeof(serverResponse), 0);       // use sizeof since everytime we init the buffer, len = 0
        if (recvStatus == -1) {
            perror("recv");
            continue;
        }
        /*
        else if (recvStatus == 0)   {
            printf("Server Disconnection");
            break;
        }
        */
        printf("%s\r\n", serverResponse);
        
    }
    
}
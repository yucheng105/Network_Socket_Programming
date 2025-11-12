#include "CreateSenderSocket.h"
#include "main.h"
#include "ServerRcv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>



int main(int argc, char *argv[])  {
    
    // input: a.out ServerIP ServerPORT
    if (argc != 3) {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        exit(1);
    }
    
    // used to connect to server
    int serverSocket = CreateSenderSocket(argv[1], argv[2]);
    
    // listen for msgs from server
    pthread_t serverListenT;
    pthread_create(&serverListenT, NULL, ServerRcv, (void *)&serverSocket);

    /////////////////////
    // User Inputs
    /////////////////////
    
    // to buffer input cmds
    char clientInput[MAXDATASIZE] = {0};

    while(1) {

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

    }



    
    
    

    // stop recv()-ing from server
    pthread_detach(serverListenT);

    close(serverSocket);
    
    
    
}
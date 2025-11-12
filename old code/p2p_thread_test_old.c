#include <string.h>
2 #include <sys/types.h>
3 #include <sys/socket.h>
4 #include <netdb.h>
5
6 #define MYPORT "3490" // the port users will be connecting to
7 #define BACKLOG 10 // how many pending connections queue holds
8
9 int main(void)
10 {
11 struct sockaddr_storage their_addr;
12 socklen_t addr_size;
13 struct addrinfo hints, *res;
14 int sockfd, new_fd;
15
16 // !! don't forget your error checking for these calls !!
17
18 // first, load up address structs with getaddrinfo():
19
20 memset(&hints, 0, sizeof hints);
21 hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
22 hints.ai_socktype = SOCK_STREAM;
23 hints.ai_flags = AI_PASSIVE; // fill in my IP for me
24
25 getaddrinfo(NULL, MYPORT, &hints, &res);
26
27 // make a socket, bind it, and listen on it:
28
29 sockfd = socket(res->ai_family, res->ai_socktype,
30 res->ai_protocol);
31 bind(sockfd, res->ai_addr, res->ai_addrlen);
32 listen(sockfd, BACKLOG);
33
34 // now accept an incoming connection:
35
36 addr_size = sizeof their_addr;
37 new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
38 &addr_size);
39
40 // ready to communicate on socket descriptor new_fd!
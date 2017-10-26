#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

const int MAX_LISTEN = 2;

vector<int> socks;

void *stream(void *args){
	int newsockfd = *(int*)args;
	socks.push_back(newsockfd);
	int n;
	char line[1024];
	time_t now = time(0);

    while((n = read(newsockfd, line, 1024)) > 0){
	        tm *ltm = localtime(&now);
            string str = line;
        	cout << "assepted " << sizeof(line) << " byts." 
        		<< " Time: "<< 1 + ltm->tm_hour << ":"
	   			<< 1 + ltm->tm_min << ":"
	   			<< 1 + ltm->tm_sec << endl;

	        for(int i = 0; i < socks.size(); i++){
	        	// cout << "socks[i] " << socks[i] << endl;
	        	if(socks[i] == newsockfd) continue;
	            if((n = write(socks[i], line, strlen(line) + 1)) < 0){
	                perror(NULL);
	                close(newsockfd);
	                exit(1);
	            }
	        }
        }
    	if(n < 0){
            perror(NULL);
            close(newsockfd);
            exit(1);
        }
        close(newsockfd);
}

int main(){
    int sockfd, newsockfd;
    socklen_t clilen;
    int n;
    char line[1024];
    struct sockaddr_in servaddr, cliaddr;
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }

    pthread_t pt;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(55555);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    
    if(listen(sockfd, MAX_LISTEN) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    write(0, "\nListening...\n", 15);
    while(1){
        clilen = sizeof(cliaddr);
        if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }

        pthread_create(&pt, NULL, stream, &newsockfd);
        pthread_detach(pt);
    }
}
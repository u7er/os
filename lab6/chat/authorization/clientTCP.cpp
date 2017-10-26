#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>


using namespace std;

#define SIZE 1024
#define SERVER "127.0.0.1"
#define PORT 55555

struct user{
	char name[10];
	char recvline[958];
	tm *ltm;

	user(){
		time_t now = time(0);
		ltm = localtime(&now);
	}
} usr;

void *stream(void *args){
	int sockfd = *(int*)args;
	char recvline[SIZE];
	int n;

	while(1){
		if ((n = read(sockfd, usr.recvline, SIZE)) < 0){
            perror("Can\'t read\n");
            close(sockfd);
            exit(1);
        }
        write(0, (struct user*) &usr, sizeof(usr));
	}
	if(n < 0){
		perror(NULL);
        close(sockfd);
        exit(1);
	}
}

int main(int argc, char **argv){
    pthread_t pt;
    int sockfd;
    int n;
    char sendline[SIZE],recvline[SIZE];
    struct sockaddr_in servaddr;
    bzero(sendline,SIZE);
    bzero(recvline,SIZE);
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_aton(SERVER, &servaddr.sin_addr) == 0){
        printf("Некорректный АЙПИ\n");
        close(sockfd);
        exit(1);
    }
    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    pthread_create(&pt, NULL, stream, &sockfd);
    pthread_detach(pt);

    cout << "Sign up (10 charset max length)" << endl;
    cin >> usr.name;

    while(1){
        printf("Сообщение: ");
        fflush(stdin);
        fgets(usr.recvline, SIZE, stdin);
        if( (n = write(sockfd, (struct user*) &usr, sizeof(usr)+1)) < 0){
            perror("Can\'t write\n");
            close(sockfd);
            exit(1);
        }
    }

    close(sockfd);
}
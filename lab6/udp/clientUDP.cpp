#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>

#define SERVER "127.0.0.1"
#define PORT 55555

int main(){
    struct sockaddr_in si_other;
    int s;
    socklen_t slen = sizeof(si_other);
	s = socket(AF_INET, SOCK_DGRAM, 0);
    	
    memset((char *) &si_other, 0, sizeof(si_other));

    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr = htonl(SERVER);
    
    // inet_aton(SERVER, &si_other.sin_addr);

    char message[1024];
    
    while(1){
		printf(" ________________\n");
		printf("/      Клиент    \\\n");
		printf("| ");
		char c;
		int i = 0;
		while((message[i++] = getchar()) != '\n');
		message[i] = '\0';
        printf("\\________________/\n");
        sendto(s, message, sizeof(message), 0 , (struct sockaddr *) &si_other, slen);
    	
		printf(" ________________\n");
		printf("/      Сервер    \\\n");
		recvfrom(s, message, sizeof(message), 0, (struct sockaddr *) &si_other, &slen);
    	printf("| %s\n", message);
    	printf("\\________________/\n");
    }

    close(s);
    return 0;
}

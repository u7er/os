#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

#define PORT 55555

int main(){
    struct sockaddr_in si_me, si_other;
    char buf[1024];
    int s, slen = sizeof(si_other);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    memset((char *) &si_me, 0, sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    bind(s, (struct sockaddr*)&si_me, sizeof(si_me));

    getsockname(s, (struct sockaddr*)&si_me, (socklen_t *) &slen);

    printf("PORT: %d\n", htons(si_me.sin_port));

    while(1){
        printf("\nКанал прослушивается...\n");
        
        recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &si_other, (socklen_t*) &slen);
		printf(" ___________________\n");
		printf("/     Данные от     \\\n");
		printf("| %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("\\___________________/\n");
		printf(" ___________________\n");
		printf("/     Сообщение     \\\n");
		printf("| %s", buf);
		printf("\\___________________/\n");
		
		int number = atoi(buf) * 2;

		stringstream oss;
		oss << number;
		string tmp = oss.str();
		
		sprintf(buf, tmp.c_str());
		sendto(s, buf, sizeof(buf), 0, (struct sockaddr*) &si_other, slen);
		printf(" ___________________\n");
		printf("/     Отправлен     \\\n");
		printf("| %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("\\___________________/\n");
		bzero(buf, sizeof(buf));
    }

    close(s);
    return 0;
}

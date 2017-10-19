#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 55555
#define SIZE 16

int main(){
    int sock, listener, status;       
    struct sockaddr_in addr, client;
    int bytes_read = 0;
    socklen_t slen = sizeof(addr);
	char buf[SIZE], buf_fname[SIZE];
	char filename[255];
	pid_t childPid, endID;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(55555);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    bind(listener, (struct sockaddr *)&addr, slen);
    
    getsockname(listener, (struct sockaddr*)&addr, &slen);
    listen(listener, 3);

    printf("PORT: %d\n", htons(addr.sin_port));

	printf("%s", filename);
    
    while(1){
		sock = accept(listener, (struct sockaddr *)&client, (socklen_t*)&slen);
		if(sock < 0){
			perror("Прием входящих подключений");
			exit(3);
		}
		
		switch(childPid = fork()){
			case -1:
				perror("fork");
				break;
			case 0:{
				recv(sock, buf_fname, SIZE, 0);
				sprintf(filename, "%s:%d_%s", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buf_fname);
				FILE *f1 = fopen(filename, "wb");
				close(listener);
				
				while(1){
					printf("Ожидаем сообщение...\n");
					bytes_read = recv(sock, buf, sizeof(buf), 0);
					if(bytes_read <= 0){
						close(sock);
						exit(EXIT_SUCCESS);
					}
					if(strcmp("OK", buf) == 0){
						printf("Файл успешно принят от клиента: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
						send(sock, buf, bytes_read, 0);
						close(sock);
						fclose(f1);
						exit(0);
					}
					
					fwrite(buf, 1, sizeof(buf), f1);
					printf("Получено %d bytes от клиента %s:%d\n", bytes_read, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
					printf("Отправляю принятое сообщение клиенту\n");
					if(send(sock, buf, bytes_read, 0) < 0){
						 close(sock);
						 exit(EXIT_SUCCESS);
					}
					memset(buf, 0, SIZE);
				}
			}
			break;
		}	    
    }
    return 0;
}

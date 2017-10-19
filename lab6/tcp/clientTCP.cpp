#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main (int argc, char **argv){
	int port;
	char message[1], filename[255], buf[1];
    int sock; 
	struct sockaddr_in addr;
	char *hostinfo;

	if(argc != 3){
		printf("Не верное кол-во аргументов!\nДолжно быть 2 аргумента (Порт,ip-адрес -сервера)!\n");
		exit(0);
	}

	port = atoi(argv[1]);
	hostinfo = argv[2];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port); 
	addr.sin_addr.s_addr = inet_addr(hostinfo);
	
	connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	
	printf("Input filename: ");
	scanf("%s", filename);
	send(sock, filename, sizeof(filename), 0); 
	FILE *f1 = fopen(filename, "rb");
	int n;
	while(1){
		n = fread(message, sizeof(char), sizeof(message), f1);
		if(n == 0){
			send(sock, "OK", 1024, 0);
			printf("Файл успешно передан\n");
			break;
		}
		else{
			printf("Отправка сообщения на сервер...\n");
			send(sock, message, sizeof(message), 0); 
			int bytes_read = 0;
			printf("Ожидание сообщения\n");
			bytes_read = recv(sock, buf, sizeof(buf), 0);
			if(bytes_read < 0){
				close(sock);
				exit(0);
			}
			else 
				printf("Получено %d bytes\tСообщение: \n%s\n", bytes_read, buf); 
		}
	}

	fclose(f1);
	close(sock);

	return 0;
}

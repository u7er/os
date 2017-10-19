#include "string.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"
#include "stdio.h"
#include "fcntl.h"

void lab1(){
	int size = 127;
	char name[size];
        char buff[size];
        memset(name, '\0', size);
        memset(buff, '\0', size);
        write(1, "What's you name?\n", 18);
        read(0, buff, size);
        strcpy(name, "Hello ");
        strcpy(name + 6, buff);
        write(1, name, size);
}

void lab2(){
	DIR *dir = opendir("/tmp");
	struct dirent *ent;
	struct stat info;
	int colwo = 0;

	while((ent = readdir(dir)) != NULL){
		colwo++;
		printf("%s", ent->d_name);
		char tmp[] = "/tmp/";
		lstat(strcat(tmp, ent->d_name), &info);
		printf("\t%d\n", info.st_size);
	}
	printf("%d\n", colwo);
}

void lab3(){
	DIR *dir = opendir("/proc/");
	struct dirent *ent;
	FILE *file;
	while((ent = readdir(dir)) != NULL){
		if(ent->d_name[0] == '1'){
			printf("%s ", ent->d_name);
			char buff[127];
			char proc[256] = "/proc/";
			strcpy(proc + 6, ent->d_name);
			strcpy(proc + 6 + strlen(ent->d_name), "/stat");
			//printf("%s", proc);
			memset(buff, '\0', 127);
			int musor;;
			file = fopen(proc, "r");
			fscanf(file, "%d %s", &musor, buff);
			printf(" %s\n", buff);
			fclose(file);
		}
	}
}

int main(){
	//lab1();
	//lab2();
	lab3();
	return 0;
}

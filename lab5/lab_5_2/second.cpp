#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct msg {
	long mtype;
	char buf[255];
};

int main() {
	key_t key = ftok("/home/metelsky/crappy/OS_temp/lab_5_2/msg.txt", 42L);
	int msgid = msgget(key, 0);
	struct msg tmp;
	tmp.mtype = 46L;
	msgrcv(msgid, (void*)&tmp, sizeof(tmp), 46L, 0);
	cout << tmp.buf;
	return 0;	
}
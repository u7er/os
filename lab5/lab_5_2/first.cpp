#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

using namespace std;

struct msg {
	long mtype;
	char buf[255];
};

int main() {
	key_t key = ftok("/home/metelsky/crappy/OS_temp/lab_5_2/msg.txt", 42L);
	int msgid = msgget(key, EPERM|IPC_CREAT);
	cout << strerror(errno);
	cout << errno;
	struct msg tmp;
	for (int i = 0; i < 255; ++i) {
		tmp.buf[i] = 'a';
	}
	tmp.mtype = 46L;
	msgsnd(msgid, (void*)&tmp, sizeof(tmp), 0);
	return 0;	
}
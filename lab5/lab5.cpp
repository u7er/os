
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <signal.h>
#include <sys/time.h>

struct itimerval nval;

int absc = -1;

void *thread_one(void *a){
	write(0, (char *)a, 1);
}

void sig() {
	absc *= -1;
	pthread_t th1;
	char a = 'A';
	pthread_t th2;
	char b = 'B';
	
	if (absc > 0) 
		pthread_create(&th1, NULL, thread_one, &a);
	else
		pthread_create(&th2, NULL, thread_one, &b);
	
	signal(SIGALRM, sig);
}

int main() {
	nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = 80000;
    nval.it_value.tv_sec = 0;
    nval.it_value.tv_usec = 80000;
    signal(SIGALRM, sig);
    setitimer(ITIMER_REAL, &nval, NULL);
	int ch;
	raise(SIGALRM);
	scanf("%d", &ch);
	//signal(SIGUSR1, raiseth1);
	//signal(SIGUSR2, raiseth2);
	/*while (scanf("%d", &ch)) {
		if (ch == 1)
			raise(SIGUSR1);
		if (ch == 2)
			raise(SIGUSR2);
		//pthread_join(th1, NULL);
	}*/
	
}

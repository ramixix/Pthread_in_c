#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <pthread.h>

void *routine(){
	printf("test from threads\n");
	sleep(3);
	printf("ending of thread\n");
}

int main(int argc, char *argv[]){
	pthread_t t1, t2;
	pthread_create(&t1, NULL, routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}


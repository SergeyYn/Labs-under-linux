#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define bufSize 5


pthread_mutex_t MCR1 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t sig21 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t forsig21 = PTHREAD_MUTEX_INITIALIZER;
sem_t SCR1;
pthread_barrier_t BCR2;

int buffer[bufSize];
int oldestItem = 0;
int newestItem = 0;


int sigFlag = 0;
int sem_count = 0;
FILE *f;

int a = 1;
int a1 = 1;
long b = 2;
long b1 = 2;
unsigned c = 3;
unsigned c1 = 3;
long unsigned d = 4 ;
long unsigned d1 = 4 ;

int globalIteration = 0;

pthread_t t1;
pthread_t t2;
pthread_t t3;
pthread_t t4;
pthread_t t5;
pthread_t t6;

void atomic_mod(){
    __sync_fetch_and_add (&a, 1);
    __sync_fetch_and_and (&a1, 2);
    __sync_fetch_and_nand (&b, 1);
    __sync_add_and_fetch(&b1, 1);
    __sync_or_and_fetch(&c, 1, 2);
    __sync_xor_and_fetch(&c1, 1);
    __sync_bool_compare_and_swap(&d, 1, 2);
    __sync_val_compare_and_swap (&d1, 1, 2);
}
void atomic_use(){
    __sync_fetch_and_add (&a, 0);
    __sync_add_and_fetch(&b, 0);
    __sync_bool_compare_and_swap(&c, c, c);
    __sync_val_compare_and_swap (&d, d, d);
}

void atomic_out(){
    fprintf(f,"Atomic variables A: %d\n", a);
    fprintf(f,"Atomic variables A1: %d\n", a1);
    fprintf(f,"Atomic variables B: %ld\n", b);
    fprintf(f,"Atomic variables B1: %ld\n", b1);
    fprintf(f,"Atomic variables C: %d\n", c);
    fprintf(f,"Atomic variables C1: %d\n", c1);
    fprintf(f,"Atomic variables D: %ld\n", d);
    fprintf(f,"Atomic variables D1: %ld\n", d1);
}

int nextInd(int i) {
    return (i + 1) % bufSize;
}

int temp_c = 0;

void add_buf(void) {
	buffer[newestItem] = temp_c;
	fprintf(f,"Element %d created on position %d\n", buffer[newestItem], newestItem);
	for(int i = 0; i < bufSize; i++){
		fprintf(f,"%d ", buffer[i]);
	}
	fprintf(f,"\n");
    newestItem = nextInd(newestItem);
    temp_c++;
}

int del = 0;
int max_del = 5;

void del_buf(void) {
	if(buffer[oldestItem] == -1){
		fprintf(f,"Can't delete empty element\n");
		oldestItem = nextInd(oldestItem);
		return;
	}
    fprintf(f,"Element %d deleted from position %d on iteration %d\n", buffer[oldestItem], oldestItem, globalIteration);
    buffer[oldestItem] = -1;
    del++;
    fprintf(f,"\n\nNUMBER OF DELETED ITEMS IS %d on iteration %d\n\n", del, globalIteration);
    for(int i = 0; i < bufSize; i++){
		fprintf(f,"%d ", buffer[i]);
	}
	fprintf(f,"\n");

    oldestItem = nextInd(oldestItem);
}

void* p1(void* unused){
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	int sem_value;
	while(1){
		globalIteration++;
		int temp = globalIteration;
		sem_getvalue(&SCR1,&sem_value);
		if(sem_value < bufSize){
			fprintf(f,"Thread 1 global iteration No.%d getting value of SCR1\n", temp);
			fprintf(f,"Thread 1 global iteration No.%d STARTED!\n", temp);
			pthread_mutex_lock (&MCR1);
			fprintf(f,"Thread 1 global iteration No.%d locked MCR1\n", temp);
			add_buf();
			pthread_mutex_unlock (&MCR1);
			fprintf(f,"Thread 1 global iteration No.%d unlocked MCR1\n", temp);
			sem_post (&SCR1);
			fprintf(f,"Thread 1 global iteration No.%d inc SCR1\n", temp);
			fprintf(f,"Thread 1 global iteration No.%d ENDED!\n", temp);
		}
	}
}
void* p2 (void* unused){
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	int sem_value;
	while(1){
		globalIteration++;
		int temp = globalIteration;
		sem_getvalue(&SCR1,&sem_value);
		if(sem_value < bufSize){
			fprintf(f,"Thread 2 global iteration No.%d getting value of SCR1\n", temp);
			fprintf(f,"Thread 2 global iteration No.%d STARTED!\n", temp);
			pthread_mutex_lock (&MCR1);
			fprintf(f,"Thread 2 global iteration No.%d locked MCR1\n", temp);
			add_buf();
			pthread_mutex_unlock (&MCR1);
			fprintf(f,"Thread 2 global iteration No.%d unlocked MCR1\n", temp);
			sem_post (&SCR1);
			fprintf(f,"Thread 2 global iteration No.%d inc SCR1\n", temp);
			pthread_mutex_lock (&forsig21);
			fprintf(f,"Thread 2 global iteration No.%d locked Msig21\n", temp);
			pthread_cond_wait(&sig21, &forsig21);
			fprintf(f,"Thread 2 global iteration No.%d recieved sig21\n", temp);
			pthread_mutex_unlock (&forsig21);
			fprintf(f,"Thread 2 global iteration No.%d unlocked Msig21\n", temp);
			fprintf(f,"Thread 2 no.%d Atomic use\n", temp);
			atomic_use();
			atomic_out();
			fprintf(f,"Thread 2 no.%d Atomic modification\n", temp);
			atomic_mod();
			atomic_out();
			fprintf(f,"Thread 2 global iteration No.%d ENDED!\n", temp);
		}
	}
}


void* p3 (void* unused){
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	while(1){
		globalIteration++;
		int temp = globalIteration;
		fprintf(f,"Thread 3 global iteration No.%d STARTED!\n", temp);
		atomic_out();
		fprintf(f,"Thread 3 no.%d Atomic modification\n", temp);
		atomic_mod();
		atomic_out();
		fprintf(f,"Thread 3 global iteration No.%d waiting for barrier\n", temp);
		pthread_barrier_wait(&BCR2);
		fprintf(f,"Thread 3 global iteration No.%d after barrier\n", temp);
		fprintf(f,"Thread 3 global iteration No.%d locked Msig21\n", temp);
		pthread_cond_signal(&sig21);
		fprintf(f,"Thread 3 global iteration No.%d sended sig21\n", temp);
		sigFlag = 1;
		pthread_mutex_unlock (&forsig21);
		fprintf(f,"Thread 3 global iteration No.%d unlocked Msig21\n", temp);
		fprintf(f,"Thread 3 no.%d Atomic use\n", temp);
		atomic_use();
		atomic_out();
		fprintf(f,"Thread 3 global iteration No.%d ENDED!\n", temp);
	}
	pthread_cancel(t2);
	pthread_cancel(t5);
	pthread_cancel(t6);
	fprintf(f,"THREADS 3, 2, 5 and 6 TERMINATED\n");
}

int isNotEmpty(){
	for(int i = 0; i<bufSize; i++){
		if(buffer[i]!=-1)
			return 1;
	}
	return 0;
}

void* p4 (void* unused){
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	int sem_value;
	while(del < max_del){
		globalIteration++;
		int temp = globalIteration;
		fprintf(f,"Thread 4 global iteration No.%d STARTED!\n", temp);
		fprintf(f,"Thread 4 global iteration No.%d waiting for SCR1\n", temp);
		sem_wait(&SCR1);
		fprintf(f,"Thread 4 global iteration No.%d recieved SCR1\n", temp);
		if(del >= max_del)
			break;
		if(isNotEmpty()){
			pthread_mutex_lock (&MCR1);
			fprintf(f,"Thread 4 global iteration No.%d locked MCR1\n", temp);
			del_buf();
			pthread_mutex_unlock (&MCR1);
			fprintf(f,"Thread 4 global iteration No.%d unlocked MCR1\n", temp);
		}
		fprintf(f,"Thread 4 global iteration No.%d ENDED!\n", temp);
	}
	pthread_cancel(t1);
	pthread_cancel(t2);
	pthread_cancel(t3);
	pthread_cancel(t5);
	pthread_cancel(t6);
	fprintf(f,"ALL THREADS TERMINATED\n");
}
void* p5 (void* unused){
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	int sem_value;
	while(del < max_del){
		globalIteration++;
		int temp = globalIteration;
		fprintf(f,"Thread 5 global iteration No.%d STARTED!\n", temp);
		sem_wait(&SCR1);
		fprintf(f,"Thread 5 global iteration No.%d recieved SCR1\n", temp);
		if(del >= max_del)
			break;
		if(isNotEmpty()){
			pthread_mutex_lock (&MCR1);
			fprintf(f,"Thread 5 global iteration No.%d locked MCR1\n", temp);
			del_buf();
			pthread_mutex_unlock (&MCR1);
			fprintf(f,"Thread 5 global iteration No.%d unlocked MCR1\n", temp);
			sem_getvalue(&SCR1,&sem_value);
			while(sigFlag!=1){
				pthread_mutex_lock (&forsig21);
				fprintf(f,"Thread 5 global iteration No.%d locked Msig21\n", temp);
				fprintf(f,"Thread 5 global iteration No.%d waiting for needed Msig21\n", temp);
				pthread_cond_wait(&sig21, &forsig21);
				pthread_mutex_unlock (&forsig21);
				fprintf(f,"Thread 5 global iteration No.%d unlocked MCsig21\n", temp);
			}
			sigFlag = 0;
			fprintf(f,"Thread 5 recieved correct sig21\n");
			fprintf(f,"Thread 5 no.%d Atomic use\n", temp);
			atomic_use();
			atomic_out();
		}
		fprintf(f,"Thread 5 global iteration No.%d ENDED!\n", temp);
		
	}
	pthread_cancel(t1);
	pthread_cancel(t2);
	pthread_cancel(t3);
	pthread_cancel(t4);
	pthread_cancel(t6);
	fprintf(f,"ALL THREADS TERMINATED\n");
}
void* p6 (void* unused){
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	while(1){
		globalIteration++;
		int temp = globalIteration;
		fprintf(f,"Thread 6 global iteration No.%d STARTED!\n", temp);
		atomic_out();
		fprintf(f,"Thread 6 no.%d Atomic modification\n", temp);
		atomic_mod();
		atomic_out();
		fprintf(f,"Thread 6 global iteration No.%d waiting for barrier\n", temp);
		pthread_barrier_wait(&BCR2);
		fprintf(f,"Thread 6 global iteration No.%d after barrier\n", temp);
		pthread_mutex_lock (&forsig21);
		fprintf(f,"Thread 6 global iteration No.%d locked Msig21\n", temp);
		pthread_cond_signal(&sig21);
		fprintf(f,"Thread 6 global iteration No.%d sended sig21\n", temp);
		sigFlag = 1;
		pthread_mutex_unlock (&forsig21);
		fprintf(f,"Thread 6 global iteration No.%d unlocked Msig21\n", temp);
		fprintf(f,"Thread 6 no.%d Atomic use\n", temp);
		atomic_use();
		atomic_out();
		fprintf(f,"Thread 6 global iteration No.%d ENDED!\n", temp);
	}
	pthread_cancel(t2);
	pthread_cancel(t3);
	pthread_cancel(t5);
	fprintf(f,"THREADS 6, 2, 5 and 3 TERMINATED\n");
	
}

int main()
{
	f = fopen("debug.log", "w");
	fprintf(f,"\n\nNEW global iteration\n\n");
	sem_init (&SCR1, 0, 0);
	for(int i = 0; i < bufSize; i++){
		add_buf();
		sem_post(&SCR1);
	}
	pthread_barrier_init(&BCR2,NULL,2);
	pthread_create (&t1, NULL, &p2, NULL);
	pthread_create (&t2, NULL, &p1, NULL);
  	pthread_create (&t3, NULL, &p3, NULL);
  	pthread_create (&t4, NULL, &p4, NULL);
  	pthread_create (&t5, NULL, &p5, NULL);
  	pthread_create (&t6, NULL, &p6, NULL);

  	pthread_join (t1, NULL);
  	pthread_join (t2, NULL);
  	pthread_join (t3, NULL);
  	pthread_join (t4, NULL);
  	pthread_join (t5, NULL);
  	pthread_join (t6, NULL);

  	fprintf(f,"\n\nWOW!\n\n");
  	fclose(f);

	return 0;
}

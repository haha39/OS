#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


//sem_t mutex;
//sem_t full;
//sem_t empty;
sem_t max_buffer_full;
sem_t min_buffer_full;
sem_t max_buffer_mutex;
sem_t min_buffer_mutex;
int big_buffer[1024];
int max_buffer[4];
int min_buffer[4];
int maximum = 0;
int minimum = 2147483647;

/*
typedef struct {
	int val;
	struct thread *L;
} semaphore;
*/

//void wait(semaphore );
//void signal(semaphore );
void *producer1();
void *producer2();
void *producer3();
void *producer4();
void *consumer1();
void *consumer2();

int main() {
	//initialized
	pthread_t p1, p2, p3, p4, c1, c2;

//	sem_init(&mutex, 0, 1);
//	sem_init(&empty, 0, 4);
//	sem_init(&full, 0, 0);
	sem_init(&max_buffer_mutex, 0, 1);
	sem_init(&max_buffer_full, 0, 0);
	sem_init(&min_buffer_mutex, 0, 1);
	sem_init(&min_buffer_full, 0, 0);

	//random
	int i;
	srand(time(NULL));
	
	for(i = 0; i < 1024; i++) {
		big_buffer[i] = rand();
	}

	for(i = 0; i < 4; i++) {
		max_buffer[i] = -1;
	}

	for(i = 0; i < 4; i++) {
		min_buffer[i] = 2147483647;
	}
	
	//create thread
	pthread_create(&p1, NULL, producer1, NULL);
	pthread_create(&p2, NULL, producer2, NULL);
	pthread_create(&p3, NULL, producer3, NULL);
	pthread_create(&p4, NULL, producer4, NULL);
	pthread_create(&c1, NULL, consumer1, NULL);
	pthread_create(&c2, NULL, consumer2, NULL);

	//join
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);
	pthread_join(c1, NULL);
	pthread_join(c2, NULL);

	//yaya
	printf("Success! maximum = %d and minimum = %d\n", maximum, minimum);

	//rest
//	sem_destroy(&mutex);
//	sem_destroy(&full);
//	sem_destroy(&empty);
	sem_destroy(&max_buffer_mutex);
	sem_destroy(&max_buffer_full);
	sem_destroy(&min_buffer_mutex);
	sem_destroy(&min_buffer_full);
	pthread_exit(NULL);
	
	return 0;
}



void *producer1() {
	int max, min, j;
	max = -1;
	min = 2147483647;

	for(j = 0; j < 256; j++) {
		if(max < big_buffer[j]) {
			max = big_buffer[j];
		}
		if(min > big_buffer[j]) {
			min = big_buffer[j];
		}
	}
	printf("Producer : Temporary max = %d and min = %d\n", max, min);

	//put max
	sem_wait(&max_buffer_mutex);	//entry cs

	max_buffer[0] = max;
	printf("Producer : put %d into max_buffer at 0\n", max);

	sem_post(&max_buffer_mutex);	//exit cs
	sem_post(&max_buffer_full);

		
	//put min
	sem_wait(&min_buffer_mutex);	//entry cs

	min_buffer[0] = min;
	printf("Producer : put %d into min_buffer at 0\n", min);

	sem_post(&min_buffer_mutex);	//exit cs
	sem_post(&min_buffer_full);

}

void *producer2() {
	int max, min, j;
	max = -1;
	min = 2147483647;

	for(j = 256; j < 512; j++) {
		if(max < big_buffer[j]) {
			max = big_buffer[j];
		}
		if(min > big_buffer[j]) {
			min = big_buffer[j];
		}
	}
	printf("Producer : Temporary max = %d and min = %d\n", max, min);

	//put max
	sem_wait(&max_buffer_mutex);	//entry cs

	max_buffer[1] = max;
	printf("Producer : put %d into max_buffer at 1\n", max);

	sem_post(&max_buffer_mutex);	//exit cs
	sem_post(&max_buffer_full);

		
	//put min
	sem_wait(&min_buffer_mutex);	//entry cs

	min_buffer[1] = min;
	printf("Producer : put %d into min_buffer at 1\n", min);

	sem_post(&min_buffer_mutex);	//exit cs
	sem_post(&min_buffer_full);
}

void *producer3() {
	int max, min, j;
	max = -1;
	min = 2147483647;

	for(j = 512; j < 768; j++) {
		if(max < big_buffer[j]) {
			max = big_buffer[j];
		}
		if(min > big_buffer[j]) {
			min = big_buffer[j];
		}
	}
	printf("Producer : Temporary max = %d and min = %d\n", max, min);

	//put max
	sem_wait(&max_buffer_mutex);	//entry cs

	max_buffer[2] = max;
	printf("Producer : put %d into max_buffer at 2\n", max);

	sem_post(&max_buffer_mutex);	//exit cs
	sem_post(&max_buffer_full);

		
	//put min
	sem_wait(&min_buffer_mutex);	//entry cs

	min_buffer[2] = min;
	printf("Producer : put %d into min_buffer at 2\n", min);

	sem_post(&min_buffer_mutex);	//exit cs
	sem_post(&min_buffer_full);
}

void *producer4() {
	int max, min, j;
	max = -1;
	min = 2147483647;

	for(j = 768; j < 1024; j++) {
		if(max < big_buffer[j]) {
			max = big_buffer[j];
		}
		if(min > big_buffer[j]) {
			min = big_buffer[j];
		}
	}
	printf("Producer : Temporary max = %d and min = %d\n", max, min);

	//put max
	sem_wait(&max_buffer_mutex);	//entry cs

	max_buffer[3] = max;
	printf("Producer : put %d into max_buffer at 3\n", max);

	sem_post(&max_buffer_mutex);	//exit cs
	sem_post(&max_buffer_full);

		
	//put min
	sem_wait(&min_buffer_mutex);	//entry cs

	min_buffer[3] = min;
	printf("Producer : put %d into min_buffer at 3\n", min);

	sem_post(&min_buffer_mutex);	//exit cs
	sem_post(&min_buffer_full);
}


void *consumer1() {
	int j;

	sem_wait(&max_buffer_full);
	sem_wait(&max_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(maximum < max_buffer[j]) {
			maximum = max_buffer[j];
		}
	}
	printf("Consumer : Updated! maximum = %d\n", maximum);
	sem_post(&max_buffer_mutex);	//exit cs

	sem_wait(&max_buffer_full);
	sem_wait(&max_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(maximum < max_buffer[j]) {
			maximum = max_buffer[j];
		}
	}
	printf("Consumer : Updated! maximum = %d\n", maximum);
	sem_post(&max_buffer_mutex);	//exit cs

	sem_wait(&max_buffer_full);
	sem_wait(&max_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(maximum < max_buffer[j]) {
			maximum = max_buffer[j];
		}
	}
	printf("Consumer : Updated! maximum = %d\n", maximum);
	sem_post(&max_buffer_mutex);	//exit cs

	sem_wait(&max_buffer_full);
	sem_wait(&max_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(maximum < max_buffer[j]) {
			maximum = max_buffer[j];
		}
	}
	printf("Consumer : Updated! maximum = %d\n", maximum);
	sem_post(&max_buffer_mutex);	//exit cs


}

void *consumer2() {
	int j;
	
	sem_wait(&min_buffer_full);
	sem_wait(&min_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(minimum > min_buffer[j]) {
			minimum = min_buffer[j];
		}
	}
	printf("Consumer : Updated! minimum = %d\n", minimum);
	sem_post(&min_buffer_mutex);	//exit cs

	sem_wait(&min_buffer_full);
	sem_wait(&min_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(minimum > min_buffer[j]) {
			minimum = min_buffer[j];
		}
	}
	printf("Consumer : Updated! minimum = %d\n", minimum);
	sem_post(&min_buffer_mutex);	//exit cs

	sem_wait(&min_buffer_full);
	sem_wait(&min_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(minimum > min_buffer[j]) {
			minimum = min_buffer[j];
		}
	}
	printf("Consumer : Updated! minimum = %d\n", minimum);
	sem_post(&min_buffer_mutex);	//exit cs

	sem_wait(&min_buffer_full);
	sem_wait(&min_buffer_mutex);	//entry cs
	for(j = 0; j < 4; j++) {
		if(minimum > min_buffer[j]) {
			minimum = min_buffer[j];
		}
	}
	printf("Consumer : Updated! minimum = %d\n", minimum);
	sem_post(&min_buffer_mutex);	//exit cs

}








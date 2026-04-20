#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "bank.h"

void *deposit(void *amount);

pthread_mutex_t mutex;

int main()
{
	int before = read_balance();
	printf("Before: %d\n", before);
	
	pthread_t threads[5];

	pthread_mutex_init(&mutex, NULL);

	int deposit_1 = 300;
	int deposit_2 = 200;


	pthread_create(&thread_1, NULL, deposit, (void*) &deposit_1);
	pthread_create(&thread_2, NULL, deposit, (void*) &deposit_2);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	pthread_mutex_destroy(&mutex);

	
	int after = read_balance();
	printf("After: %d\n", after);

	return 0;
}

void *deposit(void *amount)
{

	pthread_mutex_lock(&mutex);

	int account_balance = read_balance();
	usleep(100);
	account_balance += *((int *)amount);

	write_balance(account_balance);

	pthread_mutex_unlock(&mutex);
	return NULL;
}

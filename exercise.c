#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_coder
{
    int id;
	int compile_count;
	int is_alive;
} t_coder;

pthread_mutex_t mutex;
int total_prints = 0;

void *coder_routine(void *arg)
{
	t_coder *coders = (t_coder *)arg;
	int i = 0;
	while (i < 3){
		pthread_mutex_lock(&mutex);
		total_prints++;
		printf("coder %d esta a trabalhar (total:%d)\n", coders->id, total_prints);
		pthread_mutex_unlock(&mutex);
		usleep(1000000);
		i++;
	}
	return (NULL);
}

int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_t threads[5];
	t_coder *coder;
	coder = malloc(sizeof(t_coder) * 5);
	if (!coder){
		return (1);
	}
	int i;
	i = 0;
	while (i < 5){
		coder[i].id = i + 1;
		coder[i].compile_count = 0;
		coder[i].is_alive = 1;
		pthread_create(&threads[i], NULL, coder_routine, &coder[i]);
		i++;
	}
	i = 0;
	while(i < 5){
		pthread_join(threads[i], NULL);
		i++;
	}
	free(coder);
}



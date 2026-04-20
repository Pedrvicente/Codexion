#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_coder
{
    int id;
	int compile_count;
	int is_alive;
} t_coder;


t_coder *init_coders(int n){
	t_coder *coders;
	int i;

	coders = malloc(sizeof(t_coder) * n);
	if (coders == NULL){
		return (NULL);
	}
	i = 0;
	while (i < n){
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].is_alive = 1;
		i++;
	}
	return (coders);
}

void print_coders(t_coder *coders, int  n){
	int i;
	i = 0;
	while (i < n){
		printf("%d\n", coders[i].id);
		printf("%d\n", coders[i].compile_count);
		printf("%d\n", coders[i].is_alive);
		i++;
	}
}

void free_coders(t_coder *coders){
	free(coders);
}

int main(void)
{
	int n = 5;
	t_coder *coders = init_coders(n);
	if (!coders){
		printf("Malloc failed");
		return (1);
	}
	print_coders(coders, n);
	free_coders(coders);

    return (0);
}
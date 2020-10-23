// from the Cilk manual: http://supertech.csail.mit.edu/cilk/manual-5.4.6.pdf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

struct thread_args {
	char *config;
	int n;
	int i;
};

int safe(char *config, int i, int j) {
	int r, s;

	for (r = 0; r < i; r++) {
		s = config[r];
		if (j == s || i - r == j - s || i - r == s - j)
			return 0;
	}
	return 1;
}

int count = 0;

void nqueens(char *config, int n, int i) {
	char *new_config;
	int j;

	if (i == n) {
		sem_wait(&sem);
		count++;
		sem_post(&sem);
	}

	/* try each possible position for queen <i> */
	for (j = 0; j < n; j++) {
		/* allocate a temporary array and copy the config into it */
		new_config = malloc((i + 1) * sizeof(char));
		memcpy(new_config, config, i * sizeof(char));
		if (safe(new_config, i, j)) {
			new_config[i] = j;
			nqueens(new_config, n, i + 1);
		}
		free(new_config);
	}
	return;
}

void do_work(void *arg) {
	struct thread_args *p_in = arg;
	nqueens(p_in->config, p_in->n, p_in->i);
	return;
}

int main(int argc, char *argv[]) {
	int n;

	if (argc < 2) {
		printf("%s: number of queens required\n", argv[0]);
		return 1;
	}

	n = atoi(argv[1]);

	printf("running queens %d\n", n);

	/* INITIALIZE SEMAPHORE */
	sem_init(&sem, 0, 1);

	/* DECLARE THREAD */
	pthread_t *p_tids = malloc(sizeof(pthread_t) * n);

	/* DECLARE THREAD ARGUMENTS */
	struct thread_args *p_in[n];

	/* INITIALIZE THREAD ARGUMENTS */
	for (int i = 0; i < n; i++) {
		p_in[i] = malloc(sizeof(struct thread_args)); /* Allocate memory for each start position */
		p_in[i]->config = malloc(n * sizeof(char));
		p_in[i]->n = n;
		p_in[i]->i = 1; /* First queen has been placed */
	}

	/* CREATE THREADS */
	for (int i = 0; i < n; i++) {
		p_in[i]->config[0] = i; /* Represents our initial condition */
		pthread_create(p_tids + i, NULL, (void*) do_work, p_in[i]);
	}

	/* JOIN THREADS */
	for (int i = 0; i < n; i++) {
		pthread_join(p_tids[i], NULL);
	}

	/* FREE MEMORY */
	free(p_tids);
	for (int i = 0; i < n; i++) {
		free(p_in[i]->config);
		free(p_in[i]);
	}

	/* DESTROY SEMAPHORE */
	sem_destroy(&sem);

	printf("# solutions: %d\n", count);

	return 0;
}

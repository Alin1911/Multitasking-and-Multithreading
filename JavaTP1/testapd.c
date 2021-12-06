#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct _thread_arg {
	int thread_index;
	int thread_num;
	int array_size;
  int *v;
  int *sum_part;
  int *nr_ap_part;
	int x;
	pthread_barrier_t *barrier;
} thread_arg;


int min(int a, int b) {
    return a <= b ? a : b;
}

void *thread_func(void* args) {
    thread_arg *targs = (thread_arg *) args;

		int sum_final = 0;
		int nr_ap_final = 0;
    int thread_index = targs->thread_index;
    int thread_num = targs->thread_num;
    int array_size = targs->array_size;

    int start = thread_index * ((double) array_size / thread_num);
    int end = min(array_size, (thread_index + 1) * ((double) array_size / thread_num));

    for (int i = start; i < end; i++) {
			if(targs->x == targs->v[i]){
				targs->nr_ap_part[targs->thread_index]++;
				targs->sum_part[targs->thread_index] += i;
			}
    }

    pthread_barrier_wait(targs->barrier);

    if (targs->thread_index == 0) {
        for (int i = 0; i < thread_num; i++) {
					nr_ap_final += targs->nr_ap_part[i];
					sum_final += targs->sum_part[i];
        }

        printf("%d %d\n", nr_ap_final, sum_final);
    }
		pthread_barrier_wait(targs->barrier);
    return NULL;
}

int main(int argc, char const *argv[]) {
  if (argc < 4) {
      perror("Insufficient arguments!");
      exit(EXIT_FAILURE);
  }

  int size = atoi(argv[1]);
  int thread_num = atoi(argv[2]);
	int x = atoi(argv[3]);

  pthread_t *threads = calloc(thread_num, sizeof(pthread_t));
	thread_arg *targs = calloc(thread_num, sizeof(thread_arg));

  int *v = calloc(size, sizeof(int));
  int *sum_part = calloc(thread_num, sizeof(int));
  int *nr_ap_part = calloc(thread_num, sizeof(int));

  for (int i = 0; i < size; i++) {
      v[i] = (i + 3) % 5;
	//		printf("%d ",v[i]);
  }
	//printf("\n");
	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, thread_num);

  for (int i = 0; i < thread_num; ++i) {
		targs[i].thread_index = i;
    targs[i].array_size = size;
    targs[i].thread_num = thread_num;
    targs[i].sum_part = sum_part;
    targs[i].v = v;
    targs[i].nr_ap_part = nr_ap_part;
		targs[i].barrier = &barrier;
		targs[i].x = x;
		pthread_create(&threads[i], NULL, thread_func, &targs[i]);
	}

    for (int i = 0; i < thread_num; ++i) {
		pthread_join(threads[i], NULL);
 	}

  pthread_barrier_destroy(&barrier);
	free(targs);
	free(threads);
  free(v);
	free(sum_part);
  free(nr_ap_part);

    return 0;
}

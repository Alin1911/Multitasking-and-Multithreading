#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)

void *f(void *arg) {
  	long id = *(long*)arg;
    for (size_t i = 0; i < 100; i++) {
      printf("Hello World %ld din thread-ul %ld!\n", i, id);
    }
  	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
  int r;
  long id;
  void *status;
	long ids[NUM_THREADS];

  for (id = 0; id < NUM_THREADS; id++) {
		ids[id] = id;
		r = pthread_create(&threads[id], NULL, f, &ids[id]);
    if (r) {
	  	printf("Eroare la crearea thread-ului %ld\n", id);
	  	exit(-1);
		}
  }

  for (id = 0; id < NUM_THREADS; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  }

  	pthread_exit(NULL);
}

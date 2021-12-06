#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 1

int* arr;
int array_size;
//=======
typedef struct date {
	int start;
	int end;
} date;

void *f(void *arg) {
  	date args = *(date*)arg;

    for (size_t i = args.start; i < args.end; i++) {
    	arr[i] += 100;
    }

  	pthread_exit(NULL);
}
//=======

int main(int argc, char *argv[]) {

  	pthread_t threads[NUM_THREADS];
  	int r;
  	long id;
  	void *status;

    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    // for (int i = 0; i < array_size; i++) {
    //     printf("%d", arr[i]);
    //     if (i != array_size - 1) {
    //         printf(" ");
    //     } else {
    //         printf("\n");
    //     }
    // }

//==========
    for (id = 0; id < NUM_THREADS; id++) {
        int start = id * (double)array_size / NUM_THREADS;

        if((id + 1) * (double)array_size / NUM_THREADS < array_size){
			int end = (id + 1) * (double)array_size / NUM_THREADS;
			date args;
        	args.start = start;
        	args.end = end;
        	r = pthread_create(&threads[id], NULL, f, &args);
        	if (r) {
    	  		printf("Eroare la crearea thread-ului %ld\n", id);
    	  		exit(-1);
    		}
        }
		else{
			int end = array_size;
			date args;
        	args.start = start;
        	args.end = end;
        	r = pthread_create(&threads[id], NULL, f, &args);
        	if (r) {
    	  		printf("Eroare la crearea thread-ului %ld\n", id);
    	  		exit(-1);
    		}
        }

    }

    for (id = 0; id < NUM_THREADS; id++) {
    		r = pthread_join(threads[id], &status);

    		if (r) {
    	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
    	  		exit(-1);
    		}
    }
//============
    // for (int i = 0; i < array_size; i++) {
    //     printf("%d", arr[i]);
    //     if (i != array_size - 1) {
    //         printf(" ");
    //     } else {
    //         printf("\n");
    //     }
    // }

  	pthread_exit(NULL);
}

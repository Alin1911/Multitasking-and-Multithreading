#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;

    int number;
    int tags;
    // Checks the number of processes allowed.
    if (numtasks != 2) {
        printf("Wrong number of processes. Only 2 allowed!\n");
        MPI_Finalize();
        return 0;
    }

    // How many numbers will be sent.
    int send_numbers = 10;

    if (rank == 0) {
      for (int i = 0; i < send_numbers; i++) {
        // Generate the random numbers.
        number = rand() % (i * 50 + 2);
        // Generate the random tags.
        tags = rand() % (i * 50 + 2);
        // Sends the numbers with the tags to the second process.
        MPI_Send(&number, 1, MPI_INT, 1, tags, MPI_COMM_WORLD);
      }
    } else {
      int n = 0;
      int err = 0;
      while (n != send_numbers){
      // Receives the information from the first process.
      err = MPI_Recv(&number, 1, MPI_INT, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      // Prints the numbers with their corresponding tags.
      if (err == 0){
        n++;
      }
      printf("Number is %d tag is %d\n", number, status.MPI_TAG);
      }
    }

    MPI_Finalize();

}

#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char * argv[]) {
    int rank;
    int nProcesses;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    int nr_curent = rank * 2;
    int nr_primit_next;
    int nr_primit_prev;
    int aux;


    for (int i = 0; i < nProcesses; ++i)
    {
        if(i % 2 == 0){
            if(rank % 2 == 0)
            {   
                if(rank != nProcesses - 1){
                    MPI_Send(&nr_curent, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&nr_primit_next, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, NULL);
                    if( nr_primit_next > nr_curent){
                        aux = nr_curent;
                        nr_curent = nr_primit_next;
                    }
                }    
            } else {
                if(rank != 0){
                    MPI_Send(&nr_curent, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&nr_primit_prev, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);
                    if( nr_primit_prev <= nr_curent){
                        aux = nr_curent;
                        nr_curent = nr_primit_prev;
                    }
                }
            }
        } else {
            if(rank % 2 != 0)
            {   
                if(rank != nProcesses - 1){
                    MPI_Send(&nr_curent, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&nr_primit_next, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, NULL);
                    if( nr_primit_next > nr_curent){
                        aux = nr_curent;
                        nr_curent = nr_primit_next;
                    }
                }    
            } else {
                if(rank != 0){
                    MPI_Send(&nr_curent, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&nr_primit_prev, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);
                    if( nr_primit_prev <= nr_curent){
                            aux = nr_curent;
                            nr_curent = nr_primit_prev;
                    }
                }
            }
        }
    }

    printf("%d - %d\n",rank, nr_curent);
    if(rank == nProcesses - 1){
        int v[nProcesses];
        v[0] = nr_curent;
        for (int i = 1; i < nProcesses; i++)
        {   
            MPI_Recv(&v[i], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);
        }

        for(int i = nProcesses - 1; i >= 0; i--){
            printf("%d ", v[i]);
        }
        printf("\n");
    } else {
        for (int i = rank - nProcesses + 1; i < nProcesses; i++)
        {
            MPI_Send(&nr_curent, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            if(rank != 0)
                MPI_Recv(&nr_curent, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL);
        }
    }
    MPI_Finalize();
    return 0;
}
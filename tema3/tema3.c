#include<mpi.h>
#include<stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// printez topologia
void Print_topology(int **topology, int *len, int rank){
	printf("%d -> ", rank);
	for (int i = 0; i < 3; ++i)
	{
		printf("%d:",i);
		for (int j = 0; j < len[i] - 1; ++j)
		{
			printf("%d,", topology[i][j]);
		}
		if(i != 2)
			printf("%d ", topology[i][len[i] - 1]);
		else
			printf("%d", topology[i][len[i] - 1]);
	}
	printf("\n");
}

int main(int argc, char * argv[]){
	int rank, nProcesses;
	int *len = malloc(sizeof(int) * 3);
	int **topology = malloc(sizeof(int*) * 3);

	//initalizez vectorul de workeri
	for (int i = 0; i < 3; ++i)
	{
		len[i] = -1;
	}

	MPI_Init(&argc, &argv);
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	switch (rank){
		case 0:{
			//deschid fisierul
			FILE* fis = fopen("cluster0.txt", "r");
			if(fis == NULL){
				printf("Eroare la deschiderea fisierului cluster%d.txt", rank);
				MPI_Finalize();
				return 0;
			}
			//citesc cate subprocese are clusterul 0
			fscanf(fis, "%d", &len[0]);
			topology[rank] = calloc(sizeof(int), len[0]);
			//citesc subprocesele si le trimit liderul
			for(int i = 0; i < len[0]; i++){
				fscanf(fis, "%d", &(topology[rank][i]));
				MPI_Send(&rank, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
			}
			//trimit cate subprocese are clusterul 0 celorlalte
			MPI_Send(&(len[0]), 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			MPI_Send(&(len[0]), 1, MPI_INT, 2, 1, MPI_COMM_WORLD);

			//trimit vectorul cu subprocesele clusterului 0 celorlalte
			MPI_Send(topology[rank], len[0], MPI_INT, 1, 1, MPI_COMM_WORLD);
			MPI_Send(topology[rank], len[0], MPI_INT, 2, 1, MPI_COMM_WORLD);

			//primesc cate subprocese are clusterul 1
			MPI_Recv(&(len[1]), 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			topology[1] = calloc(sizeof(int), len[1]);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc cate subprocese are clusterul 2
			MPI_Recv(&(len[2]), 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
			topology[2] = calloc(sizeof(int), len[2]);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc subprocesele clusterului 1
			if (len[1] != -1){
				MPI_Recv(topology[1], len[1], MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}
			//primesc subprocesele clusterului 2
			if (len[2] != -1){
				MPI_Recv(topology[2], len[2], MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}

			//printez topologia
			Print_topology(topology, len, rank);

			// trimit workeri-lor restul topologiei
			for (int i = 0; i < len[0]; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					MPI_Send(&(len[j]), 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
					MPI_Send(topology[j], len[j], MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				}
			}
			fclose(fis);

			//aflu dimensiunea vectorului si o trimit celorlalte clustere
			int vector_len = atoi(argv[1]);

			MPI_Send(&vector_len, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			MPI_Send(&vector_len, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);

			//aloc doi vectori pentru calculul rezultatelor 
			int *vector   = calloc(sizeof(int),vector_len);
			int *rezultat = calloc(sizeof(int),vector_len);
			//initializez vectorul
			for (int i = 0; i < vector_len; ++i)
			{
				vector[i] = i;
			}
			// trimit vectorul celorlalte doua clustere
			MPI_Send(vector, vector_len, MPI_INT, 1, 1, MPI_COMM_WORLD);
			MPI_Send(vector, vector_len, MPI_INT, 2, 1, MPI_COMM_WORLD);

			//aflu cate subprocese sunt in topologie
			int total_worker = len[0] + len[1] + len[2];
			//un start si un end pentru fiecare subproces al clusterului curent
			int start, end;

			for (int i = 0; i < len[0]; ++i)
			{	
				//calculez start si end
				start = (topology[rank][i] - 3) * (double) vector_len / total_worker;
				end   = MIN((topology[rank][i] + 1 - 3) * (double)vector_len / total_worker, vector_len);

				//trimit vectorul, punctul de plecare si de oprire
				MPI_Send(&vector_len, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(&start, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(&end, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(vector, vector_len, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);

				//primesc rezultatul 
				MPI_Recv(vector, vector_len, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}


			for (int j = 1; j <= 2; ++j)
			{	
				//primesc rezultatele celorlalti doi clusteri
				MPI_Recv(rezultat, vector_len, MPI_INT, j, 0, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

				for (int i = 0; i < len[j]; ++i)
				{	
					//compun solutia finala stiind ce a calculat fiecare worker
					start = (topology[j][i] - 3) * (double) vector_len / total_worker;
					end   = MIN((topology[j][i] + 1 - 3) * (double)vector_len / total_worker, vector_len);
					for (int k = start; k < end; ++k)
					{
						vector[k] = rezultat[k];
					}
				}
			}

			//printez rezultatul final
			printf("Rezultat: ");
			for (int i = 0; i < vector_len -1; ++i)
			{
				printf("%d ", vector[i]);
			}
			printf("%d\n",vector[vector_len -1]);

			free(vector);
			free(rezultat);
			break;
		}
		case 1:{
			//deschid fisierul
			FILE* fis = fopen("cluster1.txt", "r");
			if(fis == NULL){
				printf("Eroare la deschiderea fisierului cluster%d.txt", rank);
				MPI_Finalize();
				return 0;
			}

			//citesc cate subprocese are clusterul 1
			fscanf(fis, "%d", &len[1]);
			topology[rank] = calloc(sizeof(int), len[1]);

			//citesc subprocesele si le trimit liderul
			for(int i = 0; i < len[1]; i++){
				fscanf(fis, "%d", &(topology[rank][i]));
				MPI_Send(&rank, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
			}
			
			//trimit cate subprocese are clusterul 1 celorlalte
			MPI_Send(&(len[1]), 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&(len[1]), 1, MPI_INT, 2, 1, MPI_COMM_WORLD);

			//trimit vectorul cu subprocesele clusterului 1 celorlalte
			MPI_Send(topology[rank], len[1], MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(topology[rank], len[1], MPI_INT, 2, 1, MPI_COMM_WORLD);

			//primesc cate subprocese are clusterul 0
			MPI_Recv(&(len[0]), 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			topology[0] = calloc(sizeof(int), len[0]);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc cate subprocese are clusterul 2
			MPI_Recv(&(len[2]), 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
			topology[2] = calloc(sizeof(int), len[2]);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc subprocesele clusterului 0
			if (len[0] != -1){
				MPI_Recv(topology[0], len[0], MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}

			//primesc subprocesele clusterului 2
			if (len[2] != -1){
				MPI_Recv(topology[2], len[2], MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}
			//printez topologia
			Print_topology(topology, len, rank);

			// trimit topologia subproceselor
			for (int i = 0; i < len[1]; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					MPI_Send(&(len[j]), 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
					MPI_Send(topology[j], len[j], MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				}
			}

			fclose(fis);

			//dimensiunea vectorului
			int vector_len;
			MPI_Recv(&vector_len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//vectorul
			int *vector = calloc(sizeof(int),vector_len);
			MPI_Recv(vector, vector_len, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//numarul total de workeri
			int total_worker = len[0] + len[1] + len[2];
			int start, end;

			for (int i = 0; i < len[1]; ++i)
			{	
				//calculez start-ul si end-ul fiecarui worker
				start = (topology[rank][i] - 3) * (double) vector_len / total_worker;
				end   = MIN((topology[rank][i] + 1 - 3) * (double)vector_len / total_worker, vector_len);
				
				//trimit start end lungimea si vectorul catre worker
				MPI_Send(&vector_len, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(&start, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(&end, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(vector, vector_len, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);

				//primesc rezultatul de la worker
				MPI_Recv(vector, vector_len, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}
			//trimit rezultatele finale procesului 0
			MPI_Send(vector, vector_len, MPI_INT, 0, 0, MPI_COMM_WORLD);

			free(vector);
			break;
		}
		case 2:{
			//deschid fisierul
			FILE* fis = fopen("cluster2.txt", "r");
			if(fis == NULL){
				printf("Eroare la deschiderea fisierului cluster%d.txt", rank);
				MPI_Finalize();
				return 0;
			}
			//citesc cate subprocese are clusterul 2
			fscanf(fis, "%d", &len[2]);
			topology[rank] = calloc(sizeof(int), len[2]);

			//citesc subprocesele si le trimit liderul
			for(int i = 0; i < len[2]; i++){
				fscanf(fis, "%d", &(topology[rank][i]));
				MPI_Send(&rank, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
			}
			
			//trimit cate subprocese are clusterul 2 celorlalte
			MPI_Send(&(len[2]), 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(&(len[2]), 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			
			//trimit vectorul cu subprocesele clusterului 2 celorlalte
			MPI_Send(topology[rank], len[2], MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Send(topology[rank], len[2], MPI_INT, 1, 1, MPI_COMM_WORLD);

			//primesc cate subprocese are clusterul 0
			MPI_Recv(&(len[0]), 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			topology[0] = calloc(sizeof(int), len[0]);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc cate subprocese are clusterul 1
			MPI_Recv(&(len[1]), 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			topology[1] = calloc(sizeof(int), len[1]);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc subprocesele clusterului 0
			if (len[0] != -1){
				MPI_Recv(topology[0], len[0], MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}

			//primesc subprocesele clusterului 1
			if (len[1] != -1){
				MPI_Recv(topology[1], len[1], MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}
			//printez topologia
			Print_topology(topology, len, rank);

			//trimit topologia workerilor
			for (int i = 0; i < len[2]; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					MPI_Send(&(len[j]), 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
					MPI_Send(topology[j], len[j], MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				}
			}
			fclose(fis);

			//dimensiunea vectorului
			int vector_len;
			MPI_Recv(&vector_len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//vectorul 
			int *vector = calloc(sizeof(int),vector_len);
			MPI_Recv(vector, vector_len, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			
			//nuamrul de workeri
			int total_worker = len[0] + len[1] + len[2];
			int start, end;

			//prarcurg workeri clusterului
			for (int i = 0; i < len[2]; ++i)
			{	
				//calculez start si end
				start = (topology[rank][i] - 3) * (double) vector_len / total_worker;
				end   = MIN((topology[rank][i] + 1 - 3) * (double)vector_len / total_worker, vector_len);

				//ii trimit workeru-lui start end si vectorul
				MPI_Send(&vector_len, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(&start, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(&end, 1, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);
				MPI_Send(vector, vector_len, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD);

				//primesc rezultatul de al worker
				MPI_Recv(vector, vector_len, MPI_INT, topology[rank][i], 0, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			}
			//trimit rezultatele finale
			MPI_Send(vector, vector_len, MPI_INT, 0, 0, MPI_COMM_WORLD);

			free(vector);
			break;
		}
		default:{
			int leader;
			//primesc liderul
			MPI_Recv(&leader, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			
			//primesc restul topologiei
			for (int i = 0; i < 3; ++i)
			{
				MPI_Recv(&(len[i]), 1, MPI_INT, leader, 0, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
				topology[i] = calloc(sizeof(int), len[i]);
				MPI_Recv(topology[i], len[i], MPI_INT, leader, 0, MPI_COMM_WORLD, &status);
				printf("M(%d,%d)\n", rank, status.MPI_SOURCE);
			}
			//printez topologia
			Print_topology(topology, len, rank);

			//primesc dimensiunea vectorului
			int vector_len;
			MPI_Recv(&vector_len, 1, MPI_INT, leader, 0, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc indicele de plecare
			int start;
			MPI_Recv(&start, 1, MPI_INT, leader, 0, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc indicele de oprire
			int end;
			MPI_Recv(&end, 1, MPI_INT, leader, 0, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//primesc vectorul
			int *vector = calloc(sizeof(int),vector_len);
			MPI_Recv(vector, vector_len, MPI_INT, leader, 0, MPI_COMM_WORLD, &status);
			printf("M(%d,%d)\n", rank, status.MPI_SOURCE);

			//workerull inmulteste
			for (int i = start; i < end; ++i)
			{
				vector[i] *= 2;
			}

			//trimit rezultatul
			MPI_Send(vector, vector_len, MPI_INT, leader, 0, MPI_COMM_WORLD);

			free(vector);
			break;
		}
	}

	free(len);
	for (int i = 0; i < 3; ++i)
	{
		free(topology[i]);
	}
	free(topology);
	MPI_Finalize();
	return 0;
}
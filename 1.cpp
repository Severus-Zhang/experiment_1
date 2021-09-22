#include <stdio.h> 
#include <mpi.h>
#include <math.h>
#include <time.h>
#define S 100000
int main(int argc, char** argv)
{
	int myid, numprocs, P, source, C = 0;
	clock_t start, end;
	double data[S], SqrtSum = 0.0, d;
	int N;
	scanf("%d", &N);
	start = clock();
	double inte;
	for (int i = 0; i < N; ++i)
	{
		data[i] = i * (i + 1);
	}
	MPI_Status status;
	char message[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	for (int i = myid; i < N; i = i + numprocs)
	{
		SqrtSum += sqrt(data[i]);
	}
	MPI_Reduce(&SqrtSum, &inte, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0)
	{
		printf("I am process 0. SqrtSum=%f.\n", inte);
	}
	MPI_Finalize();
	end = clock();           
	{
		double seconds = (double)(end - start) / CLOCKS_PER_SEC;
		fprintf(stderr, "Use time is: %.8f\n", seconds);
	}
}
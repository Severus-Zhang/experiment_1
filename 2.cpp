#define N 10000
#define a 0
#define b 10
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
int main(int argc, char** argv) {
	int myid, numprocs;
	int i;
	double data[N];
	for (i = 0; i < N; ++i)
		data[i] = i;
	MPI_Status status;
	double local = 0.0, dx = (double)(b - a) / N; 
	double inte = 0.0, x, d;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	--numprocs;
	if (myid == 0)
	{
		for (int i = 0; i < N; ++i) 
			MPI_Send(&data[i], 1, MPI_DOUBLE, i % numprocs + 1, 1, MPI_COMM_WORLD);
		for (int source = 1; source <= numprocs; ++source) 
		{
			MPI_Recv(&d, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
			inte += d;
		}
	}
	else
	{
		for (i = myid - 1; i < N; i = i + numprocs) 
		{
			MPI_Recv(&d, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
			x = a + d * dx + dx / 2;
			local += x * x * dx;
		}
		MPI_Send(&local, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD); 
	}
	if (myid == 0)
	{
		printf("The integal of x*x in region [%d,%d] =%16.15f\n", a, b, inte);
	}
	MPI_Finalize();
}
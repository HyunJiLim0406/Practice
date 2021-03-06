#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main(int argc, char *argv[]){
    int i, j;
    int A[10][10], B[2][10];
    int my_rank, size;

    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(my_rank==0){
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){
                A[i][j]=i*10+j;
            }
        }
    }
    MPI_Scatter(A, 2*10, MPI_INT, B, 2*10, MPI_INT, 0, MPI_COMM_WORLD);
    if(my_rank==3){
        for(i=0;i<2;i++){
            for(j=0;j<10;j++){
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
}
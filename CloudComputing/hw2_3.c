#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main(int argc, char *argv[]){
    int i, j;
    int A[5][5]={0}, B[5][5]={0};
    MPI_Datatype columntype;
    int my_rank, size;

    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(my_rank==0){
        for(i=0;i<5;i++){
            for(j=0;j<5;j++){
                A[i][j]=i*5+j;
            }
        }
    }
    if(my_rank==1){
        for(i=0;i<5;i++){
            for(j=0;j<5;j++){
                B[i][j]=i*5+j+50;
            }
        }
    }
    MPI_Type_vector(5,1,5,MPI_INT, &columntype);
    MPI_Type_commit(&columntype);

    if(my_rank==0){
        MPI_Bcast(&A[0][3], 1, columntype, 0, MPI_COMM_WORLD);
        MPI_Bcast(&A[0][4], 1, columntype, 1, MPI_COMM_WORLD);
    }
    else{
        MPI_Bcast(&B[0][4], 1, columntype, 0, MPI_COMM_WORLD);
        MPI_Bcast(&B[0][2], 1, columntype, 1, MPI_COMM_WORLD);
    }
    
    if(my_rank==1){
        for(i=0;i<5;i++){
            for(j=0;j<5;j++){
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
}
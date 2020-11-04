#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main(int argc, char *argv[]){
    int i, j, k, p_size;
    double R[12][12]={0.0};
    MPI_Datatype rowtype;
    int my_rank, size;

    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    p_size = 10/size;
    double **local_R = (double **)malloc(sizeof(double *)*(p_size+2));
    for(i=0;i<p_size+2;i++){
        local_R[i]=(double *)malloc(sizeof(double)*102);
    }

    if(my_rank==0){
        for(j=4;j<6;j++){
            R[0][j]=200.0;
        }
    }
    MPI_Scatter(&R[1][0], p_size*12, MPI_DOUBLE, &local_R[1][0], p_size*12, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Type_contiguous(12, MPI_DOUBLE, &rowtype);
    MPI_Type_commit(&rowtype);

    if(my_rank==0){
        for(j=0;j<=12;j++){
            local_R[0][j] = R[0][j];
        }
        MPI_Send(&local_R[p_size][0], 1, rowtype, 1, 5, MPI_COMM_WORLD);
        MPI_Recv(&local_R[p_size+1][0], 1, rowtype, 1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if(my_rank==(size-1)){
        for(j=0;j<=12;j++){
            local_R[p_size+1][j] = 0.0;
        }
        MPI_Send(&local_R[1][0], 1, rowtype, my_rank-1, 5, MPI_COMM_WORLD);
        MPI_Recv(&local_R[0][0], 1, rowtype, my_rank-1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else{
        MPI_Sendrecv(&local_R[1][0], 1, rowtype, my_rank-1, 5, &local_R[0][0], 1, rowtype, my_rank-1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&local_R[p_size][0], 1, rowtype, my_rank+1, 5, &local_R[p_size+1][0], 1, rowtype, my_rank+1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for(k=0;k<1000;k++){
        for(i=1;i<=p_size;i++){
            for(j=1;j<=10;j++){
                local_R[i][j]=0.25*(local_R[i][j-1]+R[i][j+1]+R[i-1][j]+R[i+1][j]);
            }
        }
        if(my_rank!=0){
            MPI_Sendrecv(&local_R[1][0], 1, rowtype, my_rank-1, 5, &local_R[0][0], 1, rowtype, my_rank-1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if(my_rank!=(size-1)){
            MPI_Sendrecv(&local_R[p_size][0], 1, rowtype, my_rank+1, 5, &local_R[p_size+1][0], 1, rowtype, my_rank+1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if(my_rank==0){
        for(i=0;i<p_size+2;i++){
            for(j=0;j<12;j++){
                printf("%7.1f ", local_R[i][j]);
            }
            printf("\n");
        }
    }

    free(local_R[0]);
    free(local_R);
    local_R=NULL;

    MPI_Finalize();
}
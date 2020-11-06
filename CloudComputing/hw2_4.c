#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main(int argc, char *argv[]){
    int i, j, k, p_size;
    double R[102][102]={0.0};
    MPI_Datatype rowtype; //가로로 자를 것이기 때문에 rowtype 준비
    int my_rank, size;

    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    p_size = 100/size; //프로세스의 수에 따라 각 프로세스가 계산할 행의 수
    double **local_R = (double **)malloc(sizeof(double *)*(p_size+2)); //각 프로세스의 R 동적할당
    for(i=0;i<p_size+2;i++){
        local_R[i]=(double *)malloc(sizeof(double)*100); //c
    }

    if(my_rank==0){ //R 배열 초기화
        for(j=40;j<60;j++){ 
            R[0][j]=200.0;
        }
    }
    
    
    MPI_Type_contiguous(102, MPI_DOUBLE, &rowtype); //배열의 1행에 대한 타입 정의
    MPI_Type_commit(&rowtype);
    MPI_Scatter(&R[1][0], p_size, rowtype, &local_R[1][0], p_size, rowtype, 0, MPI_COMM_WORLD); //R[1][0]부터 뿌려서 local_R[1][0]부터 저장하도록
    
    if(my_rank==0){ //0번 프로세스일 때 맨 위 가장자리의 값을 받아옴
        for(j=0;j<102;j++){
            local_R[0][j] = R[0][j];
        }
    }
    else if(my_rank==(size-1)){ //마지막 프로세스일 때 맨 아래 가장자리의 값을 받아옴
        for(j=0;j<102;j++){
            local_R[p_size+1][j] = 0.0;
        }
    }
    
    for(k=0;k<1000;k++){
        if(my_rank!=0){ //local_R의 윗부분 통신
            MPI_Sendrecv(&local_R[1][0], 1, rowtype, my_rank-1, 5, &local_R[0][0], 1, rowtype, my_rank-1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if(my_rank!=(size-1)){ //local_R의 아랫부분 통신
            MPI_Sendrecv(&local_R[p_size][0], 1, rowtype, my_rank+1, 5, &local_R[p_size+1][0], 1, rowtype, my_rank+1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        for(i=1;i<=p_size;i++){ //계산
            for(j=1;j<=100;j++){
                local_R[i][j]=(local_R[i-1][j]+local_R[i+1][j]+local_R[i][j-1]+local_R[i][j+1])/4; //0.25*()로 했을 떄는 계산이 안됐는데 ()/4로 하니까 계산이 됐습니다. 같은 말인데 왜 그랬는지 모르겠습니다.
            }
        }
    }
    
    MPI_Gather(&local_R[1][0], p_size, rowtype, &R[1][0], p_size, rowtype, 0, MPI_COMM_WORLD); //합쳐옴
    MPI_Barrier(MPI_COMM_WORLD); //다 합쳐질 때까지 대기

    if(my_rank==0){ //60~70 x 60~70 구역을 출력
        for(i=60;i<70;i++){
            for(j=60;j<70;j++){
                printf("%5.1f ", R[i][j]);
            }
            printf("\n");
        }
    }

    //local_R 동적할당 해제
    free(local_R[0]);
    free(local_R);
    local_R=NULL;

    MPI_Finalize();
}
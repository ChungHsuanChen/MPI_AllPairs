#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
void find5shortestpath(float *x, float *y,int myrank,int size){
	int i=myrank,j=0;
	int *myrank_result=(int *)malloc(sizeof(int)*(size-myrank-1));
	for(j=i+1;j<size;j++){
		float x_diff=*(x+i)-*(x+j);
		float y_diff=*(y+i)-*(y+j);
		
		printf("%d %f\n",myrank,sqrt((x_diff*x_diff)+(y_diff*y_diff)));
	}
}
int main(int argc, char * argv[]){
	int myrank,nprocs;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	float x[]={1.2,2.4,4.3,8.4,16.1};
	float y[]={1.3,2.3,4.3,8.4,16.7};
	find5shortestpath(x,y,myrank,sizeof(x)/sizeof(x[0]));
	MPI_Finalize();

}

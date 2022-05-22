#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
#include<float.h>
int compare(const void *a, const void *b)//這函式是 qsort 所需的比較函式

{
	int c = *(int *)a;
	int d = *(int *)b;
	if(c < d) {return -1;}
	else if (c == d) {return 0;}    
	else return 1;                 
}
void find5shortestpath(float *x, float *y,int myrank,int nprocs,int size){
	int i=0,j=0,counter=0,total_calculate=0;
	for(i=0;i<size;i++) total_calculate+=i;
	int pair[total_calculate][2];//Store the pair
	printf("%d\n", total_calculate);
	//Save each pair int pair array
	for(i=0;i<size-1;i++){
		for(j=i+1;j<size;j++){
			pair[counter][0]=i;
			pair[counter][1]=j;
			//printf("%d %d\n",pair[counter][0],pair[counter][1]);
			counter++;
		}	
	}
	float *myrank_result=(float *)malloc(sizeof(float)*(total_calculate/nprocs+1));
	int *pair_of_rank=(int *)malloc(sizeof(int)*(total_calculate/nprocs+1));;
	int result_size=0;
	for(i=0;i<total_calculate;i++){
		if(i%nprocs==myrank){
			float x_diff=*(x+pair[i][0])-*(x+pair[i][1]);
			float y_diff=*(y+pair[i][0])-*(y+pair[i][1]);
			float ans=sqrt((x_diff*x_diff)+(y_diff*y_diff));
			*(myrank_result+result_size)=ans;
			*(pair_of_rank+result_size)=i;
			printf("%d %d %f\n", pair[i][0],pair[i][1],ans);
			result_size++;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(myrank!=0) {
		//printf("%d\n",myrank);
		MPI_Send(&result_size,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(myrank_result,result_size,MPI_FLOAT,0,0,MPI_COMM_WORLD);
		MPI_Send(pair_of_rank,result_size,MPI_INT,0,0,MPI_COMM_WORLD);
		//printf("%d\n",myrank);
		//MPI_Barrier(MPI_COMM_WORLD);
	}
	else{
		float total_answer[total_calculate];
		int total_pair[total_calculate];
		//MPI_Barrier(MPI_COMM_WORLD);
		for(i=0;i<result_size;i++){
			total_answer[i]=myrank_result[i];
			total_pair[i]=pair_of_rank[i];
			//total_pair[i]=pair_of_rank[i];
		}
		int next=i;
		for(i=1;i<nprocs;i++){
			//printf("%d\n",i);
			MPI_Recv(&result_size,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(myrank_result,result_size,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(pair_of_rank,result_size,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			//printf("next=%d\n",next);
			for(j=0;j<result_size;j++){
				printf("now= %d %f\n",j,myrank_result[j]);
				total_answer[next]=myrank_result[j];
				total_pair[next++]=pair_of_rank[j];
			}
			
		}
		
		float original[total_calculate];
		for(i=0;i<total_calculate;i++) original[i]=total_answer[i];
		qsort((void *)total_answer,total_calculate,sizeof(total_answer[0]),compare);
		for(i=0;i<5;i++){
			//printf("%f\n",total_answer[i]);
			for(j=0;j<total_calculate;j++){
				if(total_answer[i]==original[j]){
					printf("%f (%d %d)\n",total_answer[j],pair[total_pair[j]][0]+1,pair[total_pair[j]][1]+1);
				}
			}
		}
	}
	
	free(myrank_result);	
}
int main(int argc, char * argv[]){
	int myrank,nprocs;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	float x[]={1.2,2.4,4.3,8.4,15.1};
	float y[]={1.3,2.3,4.3,8.4,16.7};
	find5shortestpath(x,y,myrank,nprocs,sizeof(x)/sizeof(x[0]));
	MPI_Finalize();

}

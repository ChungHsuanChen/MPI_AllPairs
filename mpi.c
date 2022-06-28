#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>
#include<math.h>
#include<float.h>
int compare(const void *a, const void *b)//Compare function for qsort
{
	int c = *(int *)a;
	int d = *(int *)b;
	if(c < d) {return -1;}
	else if (c == d) {return 0;}    
	else return 1;                 
}
void find5shortestpath(float *x, float *y,int myrank,int nprocs,int size){
	int i=0,j=0,counter=0;
	long long total_calculate=0;
	double t1;         
	void *v;         
	int flag;
	t1 = MPI_Wtime();         
	MPI_Comm_get_attr( MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &v, &flag );
	for(i=0;i<size;i++) total_calculate+=i;
	int pair[total_calculate][2];//Store all pairs
	printf("%d\n", total_calculate);
	//Save each pair int pair array
	for(i=0;i<size-1;i++){
		for(j=i+1;j<size;j++){
			pair[counter][0]=i;
			pair[counter][1]=j;
			counter++;
		}	
	}
	float *myrank_result=(float *)malloc(sizeof(float)*(total_calculate/nprocs+1));
	int *pair_of_rank=(int *)malloc(sizeof(int)*(total_calculate/nprocs+1));;
	int result_size=0;
	for(i=0;i<total_calculate;i++){
		if(i%nprocs==myrank){//Arrange job by modulation
			float x_diff=*(x+pair[i][0])-*(x+pair[i][1]);
			float y_diff=*(y+pair[i][0])-*(y+pair[i][1]);
			float ans=sqrt((x_diff*x_diff)+(y_diff*y_diff));
			*(myrank_result+result_size)=ans;
			*(pair_of_rank+result_size)=i;
			result_size++;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(myrank!=0) {
		MPI_Send(&result_size,1,MPI_INT,0,0,MPI_COMM_WORLD);//Send Number of job the process done
		MPI_Send(myrank_result,result_size,MPI_FLOAT,0,0,MPI_COMM_WORLD);//Send result to process:0
		MPI_Send(pair_of_rank,result_size,MPI_INT,0,0,MPI_COMM_WORLD);//Send index of pair to process:0 for retrieving pair
	}
	else{
		float *total_answer=(float *)malloc(sizeof(float)*total_calculate);
		int *total_pair=(int*)malloc(sizeof(int)*total_calculate);
		//Restore the result of process:0
		for(i=0;i<result_size;i++){
			total_answer[i]=myrank_result[i];
			total_pair[i]=pair_of_rank[i];
		}
		int next=i;
		//Receive answer from other process
		for(i=1;i<nprocs;i++){
			MPI_Recv(&result_size,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(myrank_result,result_size,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(pair_of_rank,result_size,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			for(j=0;j<result_size;j++){
				total_answer[next]=myrank_result[j];
				total_pair[next++]=pair_of_rank[j];
			}
			
		}
		
		float *original=(float*)malloc(sizeof(float)*total_calculate);
		for(i=0;i<total_calculate;i++){ 
				//printf("%f\n",total_answer[i]);
				original[i]=total_answer[i];
		}
		
		qsort((void *)total_answer,total_calculate,sizeof(total_answer[0]),compare);//sort the answer
		printf("The top 5 shortest path calculated by %d processes\n",nprocs);
		//for(i=0;i<10;i++)printf("%f ",original[i]);
		//Retrieve top 5 shortest
		for(i=0;i<5;i++){
				//printf("%f ",total_answer[i]);
			for(j=0;j<total_calculate;j++){	
				if(total_answer[i]==original[j]){
					printf("%f (%d %d)\n",total_answer[i],pair[total_pair[j]][0]+1,pair[total_pair[j]][1]+1);
				}
			}
		}
		printf("time= %f\n",MPI_Wtime()-t1);
		free(total_answer);
		free(original);
		free(total_pair);
	}
		
	free(myrank_result);	
}
void load_input(float *x,float *y, int n){
	FILE *fp=fopen("input.txt","r");
	int i;
	for(i=0;i<n;i++){
			fscanf(fp,"%f %f",&x[i],&y[i]);
			fscanf(fp,"\n");
	}
	fclose(fp);
}
int main(int argc, char * argv[]){
	int myrank,nprocs;
	void *v;         
	int flag;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	//double t0=MPI_Wtime();
	int n=10000;
	float *x=(float *)malloc(sizeof(float)*n);
	float *y=(float *)malloc(sizeof(float)*n);
	load_input(x,y,n);
	int i;
	
	//for(i=0;i<10;i++)printf("%f ",x[i]);
	find5shortestpath(x,y,myrank,nprocs,5);
	free(x);
	free(y);
	MPI_Finalize();
	//clock_t finish_time=clock();
	//double total_t=(double)(finish_time-start_time)/CLOCKS_PER_SEC;
	//printf("%d processes spend %f seconds",nprocs,total_t);
	

}

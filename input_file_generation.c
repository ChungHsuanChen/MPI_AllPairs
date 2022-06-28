#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
int main(){
	FILE *fp=fopen("input.txt","w+");
	if(!fp){
		puts("Open failed");
		return 0;
	}
	int n=10000;
	srand(time(NULL));
	while(n--){
		fprintf(fp, "%f %f\n", (float)rand()/1000.0,(float)rand()/1000.0);
	}
	fclose(fp);
	return 0;
}

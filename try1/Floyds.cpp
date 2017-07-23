#include "mpi.h"
#include<stdio.h>
#include<stdlib.h>
#define MAX 100000;

void display(int *a,int n)
{
	fprintf(stdout,"\nOUTPUT\n------\n");fflush(stdout);
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{fprintf(stdout,"%d\t",a[i*n+j]);fflush(stdout);}
		fprintf(stdout,"\n");fflush(stdout);
	}
	fprintf(stdout,"\n");fflush(stdout);
}


int main(int argc,char*argv[])
{
	int n,*a,i,j,k,rank,size,temp,*b,count;
	double t1,t2;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0)
	{
		
		n=size;count=0;
		fprintf(stdout,"The no. of nodes: %d\n",n);fflush(stdout);
		//scanf("%d",&n);
		
	}
	
		MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
		a=(int*)calloc(n*n,sizeof(int));
		b=(int*)calloc(n,sizeof(int));
	if(rank==0)
	{fprintf(stdout,"Enter the %d X %d distance matrix :\n\n(PUT -1 For INFINITY)\n\n",n,n);fflush(stdout);
		for(i=0;i<n;i++)
		{
			
			for(j=0;j<n;j++)
				{

					scanf("%d",&a[i*n+j]);
					if(i==j)
						a[i*n+j]=0;
					if(a[i*n+j]==-1)
						a[i*n+j]=MAX;
				}
		}
		t1=MPI_Wtime();
	}
	
	MPI_Bcast(a,n*n,MPI_INT,0,MPI_COMM_WORLD);
	for(k=0;k<size;k++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if(k>0)
			{
				MPI_Allgather(b,n,MPI_INT,a,n,MPI_INT,MPI_COMM_WORLD);
				//MPI_Bcast(a,n*n,MPI_INT,0,MPI_COMM_WORLD);
				if(rank==0)
				{display(a,n);}
				MPI_Barrier(MPI_COMM_WORLD);
			}
		if(k==rank)
		{
			for(i=0;i<n;i++)
				{
					b[i]=a[n*rank+i];
					if(rank==0)
						count++;
				}
			continue;
		}
		for(i=0;i<n;i++)
		{
			if(rank==0)
				count++;
			if(i==k)
			{b[i]=a[n*rank+i];continue;}
			temp=a[rank*n+k]+a[k*n+i];
			//fprintf(stdout,"%d\t%d\t%d\t%d",rank,i,a[rank*n+i],temp);fflush(stdout);
			if(temp < a[rank*n+i])
			{b[i]=temp;//fprintf(stdout,"%d\t%d\t%d\t%d",rank,i,a[rank*n+i],temp);fflush(stdout);
			}
			else
				b[i]=a[n*rank+i];			
		}


	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Allgather(b,n,MPI_INT,a,n,MPI_INT,MPI_COMM_WORLD);
	if(rank==0)
	{
		
		t2=MPI_Wtime();
		display(a,n);
		fprintf(stdout,"\nTime taken = %lf\t count=%d\n",t2-t1,count);fflush(stdout);
	}
		
	MPI_Finalize();
	return(0);
}
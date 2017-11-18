/***********************************************************************
* Program:
*    Lab Threads  
*    Brother Jones, CS 345
* Author:
*    Winson So
* Summary:
*    The program is to use threads to do matrix multiplication.
************************************************************************/
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define M 5  // number of rows in matrix A     
#define K 6  // number of rows in matrix B -- number of columns in matrix A     
#define N 8  // number of columns in matrix B


// [rows][columns]     
int A [M][K] = { {1,4,3,7,9,1},
				 {2,5,4,8,6,3}, 
				 {3,6,5,8,2,3},
				 {3,8,8,1,4,1},
				 {1,5,4,5,7,9} };     
int B [K][N] = { {1,5,6,5,7,9,8,2}, 
				 {1,2,3,5,5,6,7,8},
				 {3,5,9,7,3,1,4,1},
				 {8,3,1,2,6,5,2,4},
				 {3,8,8,1,4,1,3,3},
				 {8,7,6,5,4,3,2,1} };     
				 
int C [M][N];  // this is where the answer will be placed


/**********************************************************************
* This will calculate a cell of the final answer and put it into the
* global "C" 
***********************************************************************/
void* calculateCell(void* c)
{
   int* coord = (int*) c;
   int sum = 0;

   int i = coord[0];
   int j = coord[1];

   for (int n = 0; n < K; ++n)
   {
      sum += A[i][n] * B[n][j];
   }

   C[i][j] = sum;

   delete (coord);
   pthread_exit(0);
}

/*********************
* MAIN
*********************/
int main(int argc, char *argv[])
{
   pthread_t tid[M][N];

   // Spawn threads
   for (int i = 0; i < M; ++i)
   {
      for (int j = 0; j < N; ++j)
      {
         int* coord = new (int) (sizeof(int) * 2);

         coord[0] = i;
         coord[1] = j;

         // create the threads 
         int rc = pthread_create( &(tid[i][j] ), NULL, calculateCell, (void *) coord);
         if (rc)
         {
       		printf("ERROR; return code from pthread_create() is %d\n", rc);
       		exit(-1);
       	 }
 
      }
   }

   printf("The product of matrices A and B is:\n");

   // Join the threads and print out the table
   // Block until all threads complete
   for (int i = 0; i < M; ++i)
   {
      for (int j = 0; j < N; ++j)
      {
         // now wait for the thread to exit 
         pthread_join(tid[i][j], NULL);
         printf("%8d", C[i][j]);
      }
      printf("\n");
   }
   
   pthread_exit(0);
	return 0;
}
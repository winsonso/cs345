/***********************************************************************
* Program:
*    Lab OpenMPmatrixMult  
*    Brother Jones, CS 345
* Author:
*    Winson So
* Summary:
*    The program is to use threads to do matrix multiplication using
*    OpenMp.
*
* Conclusions: It seems like OpenMp is simpler and easier than pThread.
*
************************************************************************/
#include <iostream>
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
void calculateCell(int i, int j)
{
   int sum = 0;

   for (int n = 0; n < K; ++n)
   {
      sum += A[i][n] * B[n][j];
   }

   C[i][j] = sum;
}

/*********************
* MAIN
*********************/
int main(int argc, char *argv[])
{
   pthread_t tid[M][N];
   int threadct = 1;
   /* parse command-line arg for number of threads */
   if (argc > 1)
      threadct = atoi(argv[1]);
   
   #ifdef _OPENMP
      cout << "OMP defined, threadct = " << threadct << endl;
   #else
      cout << "OMP not defined" << endl;
   #endif
   
   // Spawn threads
   for (int i = 0; i < M; ++i)
   {
      for (int j = 0; j < N; ++j)
      {
         #pragma omp parrallel
         {
            calculateCell(i,j);
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
         printf("%8d", C[i][j]);
      }
      printf("\n");
   }
   
   return 0;
}

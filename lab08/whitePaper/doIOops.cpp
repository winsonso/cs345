/***********************************************************************
* Program:
*    Lab Threads - doIOops
*    Brother Jones, CS 345
* Author:
*    Brother Jones with pieces of code from BYU-Idaho Students 
* Summary:
*    This program simulates completing a bunch of IO that need to
*    be done.  Instead of doing actual IO, sleeps are done instead.
*    This program is given one argument that tells how many threads to 
*    use in getting the IO done.
************************************************************************/

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

// The maximum number of IO operations to be done.
const int MAX_IO_OPS = 1000;
// Initialize an array that provides the simulated IO time (sleep time)
// for the various IO operations.
const int SIMULATED_IO[MAX_IO_OPS] = 
      { 2975927, 1219003,    3781, 1530320,  482044,    8627,  768811, 2384440,
         100905, 1216700, 1867600,   80830,  414900, 2634000,     258, 1536298,
         818800, 1993134,  535600, 1277433,   26992, 1400855,   19306, 2221900,
         974122,     780,    3579,  588500,    7220,   22578,       0,    3333,
        1974927, 1219003,    3781, 1530320,  482044,    8627,  768811, 2384440,
         100905, 1216700, 1867600,   80830,  414900, 2634000,     258, 1536298,
         818800, 1993134,  535600, 1277433,   26992, 1400855,   19306, 2221900,
         974122,     780,    3579,  588500,    7220,   22578,       0 };

/***********************************************************************
* struct ThreadIOs
*  This data structure holds the start index and increment that are passed
*  to a thread to tell the thread which IO operations to work on.
************************************************************************/
struct ThreadIOs
{
   int startAt;
   int increment;
};


// Each thread is started in the doIOops function to "do some IO operations"
// (actually 'sleep' operations) for a specified number of microseconds. Each
// thread is passed a structure that tells which 'operation' to start on and
// an increment value that tells where to pick up the next 'IO operation.'
// A pointer to the structure is passed to the doIOops function.
void* doIOops(void *whichOpsToDo);

/***********************************************************************
*    This program simulates completing a bunch of IO operations that need to
*    be done.  Instead of doing actual IO, sleeps are done instead.
*    The IO operations are done by a user specified number of threads.
************************************************************************/
int main(int numArgs, char* args[])
{
   //Check for one argument that is the number of threads to run
   if (numArgs != 2)
   {
      cerr << "Usage: " << args[0] << " <# threads>" << endl;
      exit(1);
   }

   //Convert the argument from a string to a number
   int numThreads = atoi(args[1]);
   //Check that the number of threads asked for does not exceed the maximum
   //allowed for this program
   if (numThreads > MAX_IO_OPS || numThreads < 1)
   {
      cerr << args[0] << ": Number of threads allowed is 1 to " 
           << MAX_IO_OPS << endl;
      cerr << "            or, the maximum threads allowed on the system for one process.\n";
      exit(2);
   }

   // Create an array to hold all the thread IDs
   pthread_t* threads = new pthread_t[MAX_IO_OPS];
   int tidIndex = 0;  //index into the array

   //The number of threads asked for is the increment given to each thread.
   // If there are to be 10 threads, the first thread executes operations
   // 0, 10, 20, 30, ..., the second thread executes IO operations 1, 11, 21,
   // 31, ... and so on.  Each thread will be responsible to check that it
   // doesn't try to execute non-existent IO operations.

   //Create threads telling them which IO operations to do.  Each thread is
   // passed a pointer to a ThreadsIOs structure.
   for (int i = 0; i < numThreads; i++)
   {
      ThreadIOs* pThreadIOs = new ThreadIOs;
      pThreadIOs->startAt = i;
      pThreadIOs->increment = numThreads;
      pthread_create(&threads[tidIndex], NULL, doIOops, (void*)pThreadIOs);
      tidIndex++;
   }
   
   // Wait for all the threads to finish
   for (int i = 0; i < tidIndex; i++)
   {
      pthread_join(threads[i],NULL);
   }
   
   //cleanup
   delete [] threads;
}


/***********************************************************************
* Function Name: doIOops
*
* Summary: This function completes IO operations (sleep operations) that are 
*          specified in a global array called SIMULATE_IO[].
*
* Input: This function is passed a pointer to a structure that tells
*        which 'IO operation' to start on in the global array and an
*        increment value that tells where to pick up the next 'IO operation.'
*
* Output: Completes the IO (sleep) operation and prints a line of output to
*         the console.
************************************************************************/
void* doIOops(void *whichOpsToDo)
{
   // Put startAt and increment values into local variables
   int startIOs = ((ThreadIOs*) whichOpsToDo)->startAt;
   int incrBy = ((ThreadIOs*) whichOpsToDo)->increment;

   // Do the IOs
   for (int IOnum = startIOs; IOnum < MAX_IO_OPS; IOnum += incrBy)
   {
      usleep(SIMULATED_IO[IOnum]);
      cout << "Finished IO number " << IOnum << " with " 
           << SIMULATED_IO[IOnum] / 1e6 << " seconds of IO" << endl;
   }
   
   delete (ThreadIOs*) whichOpsToDo;

   pthread_exit(NULL);
}


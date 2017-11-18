/***********************************************************************
* Program:
*    Lab ProducerConsumer
*    Brother Jones, CS 345
* Author:
*    Trevor Larson
*
* Summary:
*    This program will create producer and consumer threads that share
*    a buffer. The producers will add items to the buffer and the
*    consumers will remove items ina FIFO ordering.
************************************************************************/

#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iomanip>
#include <queue>
using namespace std;

#define BUFFER_SIZE 5

// prototypes
void *producer( void *param );
void *consumer( void *param );
int insertNumber(int num);
int removeNumber(int *num);

int sleepTime;        // Parameter 1
int numPthreads;      // Parameter 2
int numCthreads;      // Parameter 3
int count;
queue<int> buffer;
pthread_mutex_t mutex;
sem_t full;
sem_t empty;
 
/************************************************************************
 * Function: Main
 * Purpose: This will create each of the producer and consumer threads
 * and also create the semaphores and mutex lock.
 ************************************************************************/
int main(int argc, char *argv[])
{
   // Exit if wrong number of parameters
   if (argc != 4)
   {
      cerr << "ERROR: Incorrect amount of parameters" << endl;
      exit(1);
   }
   // Assign values if correct amount of parameters
   else
   {
      sleepTime = atoi(argv[1]);
      numPthreads = atoi(argv[2]);
      numCthreads = atoi(argv[3]);
   }

   pthread_t producerThreads[numPthreads];
   pthread_t consumerThreads[numCthreads];
   int errorCheck = 0;
   count = 0;

   // initialize mutex lock
   pthread_mutex_init(&mutex, NULL);

   // initialize semaphores
   sem_init(&full, 0, 0);
   sem_init(&empty, 0, BUFFER_SIZE);

   cout << "Produced  by P#  Consumed  by C#" << endl;
   cout << "========  =====  ========  =====" << endl;

   int *pNum = new int[numPthreads];
   // Create each producer thread
   for (int i = 0; i < numPthreads; i++)
   {
      pNum[i] = (i + 1);
      errorCheck = pthread_create(&producerThreads[i], NULL, producer, (void*) &pNum[i]);
      if (errorCheck)
      {
         cerr << "ERROR: Failed to create thread!" << endl;
         exit(-1);
      }
   }

   int *cNum = new int[numCthreads];
   // Create each consumer thread
   for (int j = 0; j < numCthreads; j++)
   {
      cNum[j] = (j + 1);
      errorCheck = pthread_create(&consumerThreads[j], NULL, consumer, (void*) &cNum[j]);
      if (errorCheck)
      {
         cerr << "ERROR: Failed to create thread!" << endl;
         exit(-1);
      }
   }

   // Sleep for time specified by command line argument
   sleep(sleepTime);

   // Cancel producer threads
   for (int i = 0; i < numPthreads; i++)
      pthread_cancel(producerThreads[i]);

   // Cancel consumer threads
   for (int j = 0; j < numCthreads; j++)
      pthread_cancel(consumerThreads[j]);

   // Delete allocated memory
   delete [] pNum;
   delete [] cNum;
   
   return 0;
}

/************************************************************************
 * Function: producer
 * Purpose: Each producer thread will run this function and place data
 * into the buffer.
 ************************************************************************/
void *producer( void *param )
{
   //bufferItem itemProduced;
   int itemProduced;
   while (true)
   {
      // sleep for random amount of time
      usleep(rand() % 150000);
      // generate a random number between 0 and 999
      itemProduced = (rand() % 1000);
      sem_wait(&empty);
      pthread_mutex_lock(&mutex);
      // Enter critical section
      int temp = insertNumber(itemProduced);

      if (temp == -1)
         cerr << "Error inserting number into buffer!" << endl;
      else
      {
         cout << setw(5) << itemProduced << setw(8) << "P" << *((int *)param)
              << endl;
      }
      // Exit critical section
      pthread_mutex_unlock(&mutex);
      sem_post(&full);
   }
}

/************************************************************************
 * Function: consumer
 * Purpose: Each consumer will run this function and remove data from
 * the buffer.
 ************************************************************************/
void *consumer( void *param )
{
   //bufferItem consumeItem;
   int consumeItem;
   while (true)
   {
      // sleep for random amount of time
      usleep(rand() % 150000);
      sem_wait(&full);
      pthread_mutex_lock(&mutex);
      // Enter critical section
      int temp = removeNumber(&consumeItem);

      if (temp == -1)
         cerr << "Error removing number from buffer!" << endl;
      else
      {
         cout << setw(22) << consumeItem << setw(8) << "C" << *((int *)param)
              << endl;
      }
      // Exit critical section
      pthread_mutex_unlock(&mutex);
      sem_post(&empty);
   }
}

/************************************************************************
 * Function: insertNumber
 * Purpose: This function checks to see if the buffer is full and will
 * insert a number into it if it is not full.
 ************************************************************************/
int insertNumber(int num)
{
   // Check if the buffer is full
   if (count < BUFFER_SIZE)
   {
      // add item to buffer and increment value
      buffer.push(num);
      count++;
      return 1;
   }
   else
      return -1;
}

/************************************************************************
 * Function: removeNumber
 * Purpose: This function will remove a number from the buffer as long as
 * it is not empty.
 ************************************************************************/
int removeNumber(int *num)
{
   // Check if buffer is empty
   if (count > 0)
   {
      // remove item from buffer and decrement value
      *num = buffer.front();
      buffer.pop();
      count--;
      return 1;
   }
   else
      return -1;
}

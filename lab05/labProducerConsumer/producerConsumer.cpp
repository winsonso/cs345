/****************************************************************************
* Program:
*    Lab ProducerConsumerT2
*    Brother Jones, CS 345
* Author:
*    Winson So
*
* Summary:
*    This program is to implement the producer consumer pattern.
*    It has a shared buffer of values that have been produced and
*    can be consumed. This will implement a First In First Out (FIFO) queue 
*
************************************************************************
*
* Changes made to my code for the re-submission:
* nothing changed at all
*
*****************************************************************************/

#include <pthread.h> 
#include <stdio.h>
#include <string>
#include <unistd.h> 
#include <stdbool.h> 
#include <stdlib.h> 
#include <semaphore.h>

typedef int bufferItem;
#define BUFFER_SIZE 5

bufferItem buffer[BUFFER_SIZE];

// semaphore telling if there's room enough to save produced item
sem_t canProduce;
// semaphore telling if there's anything to consume
sem_t canConsume;

pthread_mutex_t myMutex;

// These are keep track of which spot is next to 
// be consumed or produced
int global_pi = 0; // producer index
int global_ci = 0;  // consumer index

/***********************************************************************
 * This is the producer thread function. It will produce values and place
 * them into the buffer
 ************************************************************************/
void* producer (void* arg)
{
   int pNum = *((int*) arg);
   free((int*) arg);
   bufferItem itemProduced;
   while (true)
   {
      /* sleep for a random period of time */
      usleep(rand() % 150000);

      /* generate a random number */
      itemProduced = (rand() % 1000);

      /* insert item into shared buffer and print what was done */
      {
         int i;
         sem_wait(&canProduce);

         pthread_mutex_lock(&myMutex);
         i = global_pi;
         global_pi = (global_pi + 1) % BUFFER_SIZE;
         pthread_mutex_unlock(&myMutex);

         buffer[i] = itemProduced;
         printf("%5d     P%d\n", itemProduced, pNum);

         sem_post(&canConsume);
      }

   }

}
/***********************************************************************
 * This is a function that will consume items in the buffer 
 * and print them on the command line
 ************************************************************************/
void* consumer(void *arg)
{
   int pNum = *((int*) arg);
   free((int*) arg);

   while (true)
   {
      // Consume an item
      {
         // wait if there's no items to consume
         sem_wait(&canConsume);
         int i = 0;
         {
            pthread_mutex_lock(&myMutex);
            i = global_ci;
            global_ci = (global_ci + 1) % BUFFER_SIZE;
            pthread_mutex_unlock(&myMutex);
         }

         // get item to consume
         bufferItem next = buffer[i];
         buffer[i] = -1; // clear it. Helps debug.
         sem_post(&canProduce); // tell others there's a blank spot

         // Print out what we did.
         printf("%19d     C%d\n", next, pNum);
      }


      /* sleep for a random period of time */
      usleep(rand() % 150000);
   }

}
/***********************************************************************
 * Starts off a bunch of producer and consumer threads and sets everything
 * up
 ************************************************************************/
int main (int argc, char* argv[])
{

   /* 1. Check and get command line arguments argv[1], argv[2], argv[3] */
   int i;

   if (argc != 4) {
      printf("Error: You need to pass 3 arguments.\n");
      exit(1);
   }
   int sleep_time = atoi(argv[1]);
   int num_producers = atoi(argv[2]);
   int num_consumers = atoi(argv[3]);

   pthread_t* producer_ids  = new pthread_t[num_producers];
   pthread_t* consumer_ids = new pthread_t[num_consumers];

   /* 2. Initialize buffer [good for error checking but not really needed]*/
   for (i = 0; i < BUFFER_SIZE; i++) {
      buffer[i] = -1;
   }

   /* 3. Initialize the mutex lock and semaphores */
   pthread_mutex_init(&myMutex, NULL);

   /* Initialize the semaphores */
   sem_init(&canProduce, 0, BUFFER_SIZE);
   sem_init(&canConsume, 0, 0);

   printf("Produced by P# Consumed by C#\n");
   printf("======== ===== ======== =====\n");


   /* 4. Create producer threads(s) */
   for (i = 0; i < num_producers; i++) {

      int* id = new int;
      *id = i + 1;

      int err = pthread_create( &producer_ids[i], NULL, producer, (void*) id);
      if(err)
      {
      	printf("Error creating producer thread\n");
      	exit(1);	
      }
   }

   /* 5. Create consumer threads(s) */
   for (i = 0; i < num_consumers; i++) {

      int* id = new int;
      if (id == NULL) {
         fprintf(stderr, "no memory for id");
         exit(EXIT_FAILURE);
      }

      *id = i + 1;

      int err = pthread_create( &consumer_ids[i], NULL, consumer, (void*) id);
      if(err)
      {
      	printf("Error creating consumer thread\n");
      	exit(1);	
      }
   }

   /* 6. Sleep [ to read manual page, do: man 3 sleep ]*/
   sleep(sleep_time);

   /* 7. Cancel threads [ pthread_cancel() ] */
   for (i = 0; i < num_producers; i++) {
      int err = pthread_cancel(producer_ids[i]);
      if(err)
      {
      	printf("Error canceling producer thread\n");
      	exit(1);	
      }
   }
   for (i = 0; i < num_consumers; i++) {
      int err = pthread_cancel(consumer_ids[i]);
      if(err)
      {
      	printf("Error canceling consumer thread\n");
      	exit(1);	
      }
   }
   
   /* 8. Exit */
   delete [] consumer_ids;
   delete [] producer_ids;
   return 0;

}

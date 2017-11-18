/****************************************************************************
 *  This version of pthread_ex_OSC.c shows two sets of changes to the code.
 *  One change is how the runner function is defined and the other is using
 *  an int to pass the parameter to the runner function.
 *
 *  A diff showing the just the runner definition chnages is:
 *  
 *  [prompt ~]# diff pthread_ex_OSC.c pthread_ex_OSC_ver1.c
 *  6c6
 *  < void *runner(void *param);
 *  ---
 *  > void runner(void *param);
 *  28c28
 *  <    pthread_create(&tid, &attr, runner, argv[1]);
 *  ---
 *  >    pthread_create(&tid, &attr, (void *) &runner, argv[1]);
 *  37c37
 *  < void *runner(void *param)
 *  ---
 *  > void runner(void *param)
 *
 *  A diff showing using an int instead of argv[1]:
 *  18,21c18
 *  <
 *  <    // Convert input string to an integer and use integer value
 *  <    int upperLimit = atoi(argv[1]);
 *  <    if (upperLimit < 0)
 *  ---
 *  >    if (atoi(argv[1]) < 0)
 *  31c28
 *  <    pthread_create(&tid, &attr, runner, (void *) &upperLimit);
 *  ---
 *  >    pthread_create(&tid, &attr, runner, argv[1]);
 *  42c39
 *  <    int upper = *((int *) param);
 *  ---
 *  >    int upper = atoi(param);
 *
 *
 ****************************************************************************/

#include <stdio.h>
#include <pthread.h>

int sum = 0;			/* this data is shared by the thread(s) */
void runner(void *param);	/* the thread */

int main(int argc, char *argv[])
{
   pthread_t tid;		/* declare tid to be the thread identifier */
   pthread_attr_t attr;		/* declare attr to hold thread attributes */

   if (argc != 2) 
   {
      fprintf(stderr, "Usage: a.out <interger value>\n");
      return 1;
   }
   
   /* Convert input string to an integer and use integer value */
   int upperLimit = atoi(argv[1]);
   if (upperLimit < 0) 
   {
      fprintf(stderr, "Interger value must be >= 0\n");
      return 2;
   }

   /* get the default attributes */
   pthread_attr_init(&attr);

   /* create the thread */
   pthread_create(&tid, &attr, (void *) &runner, (void *) &upperLimit);

   /* now wait for the thread to exit */
   pthread_join(tid, NULL);
   printf("sum = %d\n",sum);

   return 0;
}

void runner(void *param) 
{
   int upper = *((int *) param);
   int i;
   sum = 0;

   for (i = 1; i <= upper; i++)
      sum += i;

   pthread_exit(0);
}


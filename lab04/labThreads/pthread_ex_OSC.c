
#include <stdio.h>
#include <pthread.h>

int sum = 0;			/* this data is shared by the thread(s) */
void *runner(void *param);	/* the thread */

int main(int argc, char *argv[])
{
   pthread_t tid;		/* declare tid to be the thread identifier */
   pthread_attr_t attr;		/* declare attr to hold thread attributes */

   if (argc != 2) 
   {
      fprintf(stderr, "Usage: a.out <interger value>\n");
      return 1;
   }
   if (atoi(argv[1]) < 0) 
   {
      fprintf(stderr, "Interger value must be >= 0\n");
      return 2;
   }

   /* get the default attributes */
   pthread_attr_init(&attr);

   /* create the thread */
   pthread_create(&tid, &attr, runner, argv[1]);

   /* now wait for the thread to exit */
   pthread_join(tid, NULL);
   printf("sum = %d\n",sum);

   return 0;
}

void *runner(void *param) 
{
   int upper = atoi(param);
   int i;
   sum = 0;

   for (i = 1; i <= upper; i++)
      sum += i;

   pthread_exit(0);
}


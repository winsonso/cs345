 /***********************************************************************
* Program:
*    Lab UnixShellT2, Operating Systems
*    Brother Jones, CS 345
* Author:
*    Winson So
* Summary:
*    This is a Unix Shell with a history feature on it.
*****************************************************************************
*
* Changes made to my code for the re-submission:
*   (examples:)
*   -I fixed the r commands.
*
*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <string.h>
#include <deque>
#include <string>
#include <iomanip>
#include <set>

using namespace std;

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
/* The size of the history buffer */
#define HIST_SIZE 10

struct commandObj
{
	int historyCount;
	string command;
};

// A global array of strings and a count of the number of valid strings in it.
deque<commandObj> commands; 

//A global counter;
int totalCount;      
/*************************************************************************
* add2History() adds a command to the history buffer and increments the
* number of commands that have been entered up to a maximum count.  If the
* history buffer is full, the oldest command is overwritten.    
**************************************************************************/
void add2History (string input, int length)
{
	commandObj aCommand;
   	string cli = input;
   	cli[length] = '\0';
   	aCommand.historyCount = totalCount;
   	totalCount++;
   	aCommand.command = cli;
   
	if(commands.size() < HIST_SIZE)
	{
		commands.push_back(aCommand);
   	}
   	else
   	{
   		commands.pop_front();
   		commands.push_back(aCommand);
   	}

}

/*************************************************************************
* Display the contents of the history buffer of commands with the number
* of the command listed on the beginning of the line.
**************************************************************************/
void displayHistory( )
{
   const char* message = "\n";
   write(STDOUT_FILENO, message, strlen(message));
   
   int start;

	int i;
   for (std::deque<commandObj>::iterator it = commands.begin(); it!=commands.end(); ++it)
   {            
      string tmp = (*it).command;
      char temp[255];
      sprintf(temp, " %d. %s", (*it).historyCount, tmp.c_str());
      i++;
      write(STDOUT_FILENO, temp, strlen(temp));
   }

   
   return;
}

/***************************************************************************
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters.
 *
 * setup() modifies the inputBuffer creating a set of null-terminated strings
 * and places pointers into the args[] array that point to the beginning of
 * each argument.  A NULL pointer is placed in the args[] array indicating
 * the end of the argument list.  This is what is needed for using execvp().
 *
 * A ^d input at the beginning of a line, by a user, exits the shell.
 ***************************************************************************/
void setup(char inputBuffer[], char *args[], int *background)
{
   int length; /* # of characters in the command line */
   int i;      /* loop index for accessing inputBuffer array */
   int start;  /* index where beginning of next command parameter is */
   int ct;     /* index of where to place the next parameter into args[] */
    
   ct = 0;
    
   /* read what the user enters on the command line */
   length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  

   start = -1;            /* Used as a flag to indicate that we are looking
                           * for the start of the command or an argument if
                           * it is a -1, or stores the starting position of
                           * the command or argument to be put in args[].
                           */
   if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
   if (length < 0)
   {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
   }
	
	// Add functionality on the 'r'
	if(inputBuffer[0] == 'r')
	{
		if(inputBuffer[1] == '\n')
		{
			string lastItem = (*(commands.end()-1)).command;

			char tmp2[32];
     		sprintf(tmp2, "%s", lastItem.c_str());
      		strncpy(inputBuffer, lastItem.c_str(), strlen(lastItem.c_str()));
     		length = strlen(tmp2);          	
    		inputBuffer[strlen(tmp2)-1] = '\n';
    		inputBuffer[strlen(tmp2)] = '\0'; 
    		add2History( (string) inputBuffer, length );

		}
		else if(inputBuffer[1] == ' ')
		{
			char secKeyWord = inputBuffer[2];
			set<string> tmp;
			string redoCommand;
			int find = 0;
			for(int i = commands.size()-1; i >= 0; i--)
			{
				if(commands[i].command[0] == secKeyWord)
				{
					find++;	
					// find the right one and redo again
					redoCommand = commands[i].command;
				
					char tmp2[32];
     				sprintf(tmp2, "%s", redoCommand.c_str());
      				strncpy(inputBuffer, redoCommand.c_str(), strlen(redoCommand.c_str()));
     				length = strlen(tmp2);          	
    				inputBuffer[strlen(tmp2)-1] = '\n';
    				inputBuffer[strlen(tmp2)] = '\0'; 
    				add2History( (string) inputBuffer, length );
					break;	
				}
			}
			
				if (find == 0)
				{
					const char* message = "No matching command in history\n";
  					write(STDOUT_FILENO, message, strlen(message));
  					return;
				}			
		}
	}
	else
	{
		add2History( (string) inputBuffer, length );
	}
	
   /* examine every character in the inputBuffer */
   for (i=0;i<length;i++)
   { 
      switch (inputBuffer[i])
      {
         case ' ':
         case '\t' :               /* argument separators */
            if(start != -1)        /* found the end of the command or arg */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */
               ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

         case '\n':                 /* should be the final char examined */
            if (start != -1)        /* if in the command or an argument */
            {
               args[ct] = &inputBuffer[start]; /* set up pointer in args[] */ 
               ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

         default :             /* some other character */
            if (start == -1 && inputBuffer[i] != '&')
               start = i;      /* starting position of the command or arg */
            if (inputBuffer[i] == '&')
            {
               *background  = 1;
               inputBuffer[i] = '\0';
            }
      }  /* end of switch */
   }  /* end of for loop looking at every character */
   args[ct] = NULL; /* just in case the input line was > 80 */
} 
/******************************************************
* signal for ^\
******************************************************/
void handle_SIGQUIT( int junk )
{
	if(!commands.empty())
	{
		displayHistory( );
	}
   return; 
}

/*******************
* MAIN
*******************/
int main(void)
{
   char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
   int background;             /* equals 1 if a command is followed by '&' */
   char *args[MAX_LINE/2];   /* command line (of 80) has max of 40 arguments */
    
      /* set up the signal handler */
    struct sigaction handler;
    handler.sa_handler = handle_SIGQUIT;   /* function the handler will call */
    handler.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &handler, NULL);    /* handler to call for quit signal */

   /* set up the message we want printed when USR1 is seen */
   //strncpy( msg4handler2print, "Caught signal USR1\n", BUFFER_SIZE );
    
   while (1)                   /* Program terminates normally inside setup */
   {
      background = 0;
      printf(" COMMAND-> ");
      fflush(stdout);

      
      setup(inputBuffer, args, &background);       /* get next command */

      /* the steps are:
       * (1) fork a child process using fork()
       * (2) the child process will invoke execvp()
       * (3) if background != 1, the parent will wait,
       *     for the command to complete; otherwise,
       *     setup() gets called to process the next command line. 
       */ 
       pid_t pid = fork();
       
       if (pid == 0)
	 	{
		  	// child process
		  	int status = execvp(args[0], args);
		  	if(inputBuffer[0] != 'r' || (inputBuffer[0] != 'r' && inputBuffer[1] != ' '))
		  		printf("command not found\n");
		  	return 1;

	 	}
	 	else if (pid > 0)
	 	{
		  	// parent process
         	// Wait or continue based on background
         	if (background != 1) {
            	waitpid(pid, NULL, 0);
         	}

	 	}
	 	else
	 	{
		  // fork failed
		  printf("fork() failed!\n");
		  return 1;
    	}
   }
}

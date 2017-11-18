/***********************************************************************
* Program:
*    Lab UnixShell
*    Brother Jones, CS 345
* Author:
*    David Brewerton
* Summary:
*     This creates a shell that takes in commands and executes them as
*    shells and also implement forc() and execvp() using c and c++
************************************************************************/

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <cstdlib>


#define MAX_NUM_CMDS 1000

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define BUFFER_SIZE 50

/* The size of the history buffer */
#define HIST_SIZE 10
/* Maximum number of commands until count rolls over. */


using namespace std;

// A global array of strings and a count of the number of valid strings in it.
string commands[HIST_SIZE];
int goodStringCount = 0;

static char msg4handler2print[BUFFER_SIZE];


/*   note: If compiling this with gcc the definition may be:
 *             void handle_SIGUSR1()
 *         For g++ this works:   void handle_SIGUSR1( int )
 *         Both gcc and g++ like:   void handle_SIGUSR1( int junk )
 */
void handle_SIGQUIT( int junk )
{
   int lenOfMsg;
   lenOfMsg = strnlen(msg4handler2print, BUFFER_SIZE);
   write(STDOUT_FILENO, msg4handler2print, lenOfMsg);
   /* this handler could exit, but typically would return so that you
    * continue execution of the program after handling the signal. */
   //exit(0);
   return;
}

/**
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters.
 *
 * setup() modifies the inputBuffer creating a set of null-terminated strings
 * and places pointers into the args[] array that point to the beginning of
 * each argument.  A NULL pointer is placed in the args[] array indicating
 * the end of the argument list.  This is what is needed for using execvp().
 *
 * A ^d input at the beginning of a line, by a user, exits the shell.
 */

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
/**************************************************************
*   spawn a child to make sure that the fork works properly
***************************************************************/
int spawn(char* program, char** arg_list)
{
   pid_t child_pid;
   /* Duplicate this process. */
   child_pid = fork ();
   if (child_pid != 0)
   {
      /* This is the parent process. */
      return child_pid;
   }
   else
   {
      /* Now execute PROGRAM, searching for it in the path. */
      execvp (program, arg_list);
      /* The execvp function returns only if an error occurs. */
      fprintf (stderr, "command not found\n");
      abort ();
   }
}
/*************************************************************************
* add2History() adds a command to the history buffer and increments the
* number of commands that have been entered up to a maximum count.  If the
* history buffer is full, the oldest command is overwritten.
**************************************************************************/
void add2History (string input)
{
   commands[goodStringCount % HIST_SIZE] = input;
   goodStringCount++;

   // Roll over the count if it hits the maximum allowed for printing
   if ( goodStringCount >= MAX_NUM_CMDS )
      goodStringCount = HIST_SIZE;    /* Leave a full buffer of commands */
}

/*************************************************************************
* findCommandInHistory() searches backward for the first command that
* begins with a particular character (char c) in the history buffer.
* If a string is found that begins with the character, the string is
* passed back to the calling function.  If the character being looked
* for is a NULL then the most recent command is returned.  In both of
* the preceeding cases, the return value is set to true.  If no matching
* string is found, the return value is set to false.
**************************************************************************/
bool findCommandInHistory(char c, string& stringFound)
{
   int temp = goodStringCount;
   stringFound = "";

   // If the character being looked for is a null, then return the most
   // recent command, if there was one.
   if (c == '\0')
   {
      if (goodStringCount)
      {
         stringFound = commands[ (goodStringCount - 1) % HIST_SIZE ];
         return true;
      }
      else
      {
         return false;
      }
   }

   // Look back through the history for a command staring with the letter
   // given in the first argument to this function.
   for (temp; temp >= 0 && temp != temp - HIST_SIZE; --temp)
   {
      if (commands[temp % HIST_SIZE][0] == c)
      {
         stringFound = commands[temp % HIST_SIZE];
         return true;
      }
   }

   return false;
}


/*************************************************************************
* Display the contents of the history buffer of commands with the number
* of the command listed on the beginning of the line.
**************************************************************************/
void displayHistory( )
{
   int start;

   if (goodStringCount > HIST_SIZE)
      start = goodStringCount - HIST_SIZE;
   else
      start = 0;

   for (int i = start; i < goodStringCount; ++i)
   {
      cout << " " << setw(3) << i << "  " << commands[i % HIST_SIZE] << endl;
   }

   return;
}





int main(void)
{
   // Start of shellHistory variables
   string tmpString;
   int length = -1;
   bool goodCmd = true;

   // Start of shell and SIGQUIT sighandler catch
   char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
   int background;             /* equals 1 if a command is followed by '&' */
   char *args[MAX_LINE/2];   /* command line (of 80) has max of 40 arguments */
   struct sigaction handler;
   handler.sa_handler = handle_SIGQUIT;   /* function the handler will call */
   sigaction(SIGQUIT, &handler, NULL);    /* handler to call for USR1 signal */
   /* set up the message we want printed when USR1 is seen */
   strncpy( msg4handler2print, "Caught signal SIGQUIT\n", BUFFER_SIZE );
   pid_t child_pid;

// Start of shell and shellhistory
   while (1)                   /* Program terminates normally inside setup */
   {
     length = -1;
     goodCmd = true;
     while (length < 0)
     {
      printf(" COMMAND-> ");
      fflush(stdout);
      length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
      inputBuffer[length - 1] = '\0';
     }
     if ( length == 1 )
     {
        // If the user just entered a newline, then the length of the
        // input will be 1 and we just want to give the prompt again.
        // (Ignoring the newline input.)
        goodCmd = false;
     }
     else if (inputBuffer[0] == '.' && inputBuffer[1] == '\0')
     {
        displayHistory( );
        goodCmd = false;
     }
     else if ( length == 0 || (! strncmp( inputBuffer, "exit", MAX_LINE )) )
     {
        // End of file or exit command given
        exit(0);
     }
     else if (inputBuffer[0] == 'r' &&
                    (inputBuffer[1] == '\0' || inputBuffer[1] == ' ') )
     {
        // If an 'r' command was given and a matching command is found in the
        // history, then put the command to be repeated in the buffer.
        // If an 'r' was given with no argument the most recent command
        // should be executed.  This is indicated with a null.
        if (inputBuffer[1] == '\0') inputBuffer[2] = '\0';

        goodCmd = findCommandInHistory(inputBuffer[2], tmpString);

        if ( goodCmd )
        {
           strncpy (inputBuffer, tmpString.c_str(), MAX_LINE);
        }
        else
        {
           cout << "Command not found\n";
           cout.flush();
        }

     }

     if ( goodCmd )
     {
        add2History( (string) inputBuffer );
        cout << "  command executed: " << inputBuffer << endl;
     }
      setup(inputBuffer, args, &background);       /* get next command */
      //spawn will create a child and run the other options
      child_pid = spawn(args[0], args);
      waitpid(child_pid, NULL, 0); //wait till the program is done

   }
}

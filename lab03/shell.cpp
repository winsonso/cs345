/***********************************************************************
 * Program:
 *    Lab UnixShell
 *    Brother Jones, CS 345
 * Author:
 *    Stephen Jones
 * Summary:
 *    This program will imitate a shell program with some added
 * functionality if you type ctrl+\ it will show you the history.
 * if you type r it will redo the last done command. It uses the
 * fork and exec to run commands. it also can run in the background
 * if given an & at the end.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <iomanip>
#include <cstring>
#include <wait.h>
#include <cstdlib>
#include <sstream>
using namespace std;
#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define HIST_SIZE 10
#define MAX_NUM_CMDS 1000

static char msgHandler[MAX_LINE];
static bool signaled = false;
string commands[HIST_SIZE];
int goodStringCount = 0;
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
   stringstream result;
   if (goodStringCount > HIST_SIZE)
      start = goodStringCount - HIST_SIZE;
   else
      start = 0;
   cout << endl;
   for (int i = start; i < goodStringCount; ++i)
   {
      cout <<  " " << setw(3) << i << "  " << commands[i % HIST_SIZE] << endl;
      
      // result << " " << setw(3) << i << "  " << commands[i % HIST_SIZE] << endl;
   }

   return;// result.str().c_str();
}

/**
 *
 */
void handle_SIGQUIT(int junk )
{
   // int messageLength;
   // messageLength = strnlen(msgHandler,MAX_LINE);
   //write(STDOUT_FILENO, msgHandler, messageLength);
   displayHistory();
   signaled = true;
   return;


}

/**
 * setup() reads in the next command line, separating it into distinct token\
 s
 * using whitespace as delimiters.
 *
 * setup() modifies the inputBuffer creating a set of null-terminated string\
 s
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
   pid_t pid;
   int status = 0;
   bool okToAdd = true;
   string temp;
   string tempString;
   ct = 0;
   // cout << inputBuffer << " input buffer" << endl;
   //cout << "args: " << args;
   /* read what the user enters on the command line */
   length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
   fflush(stdout);
   start = -1;            /* Used as a flag to indicate that we are looking
                           * for the start of the command or an argument if
                           * it is a -1, or stores the starting position of
                           * the command or argument to be put in args[].
                           */
   if (length == 1)
      okToAdd = false;
   if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
   if (length < 0 && !signaled)
   {
      perror("Error reading the command");
      exit(-1);           /* terminate with error code of -1 */
   }
   
   /* examine every character in the inputBuffer */
   if ( !signaled )
   {
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
      }  /*end of for loop looking at every character */
    
      if ( inputBuffer[0] == 'r' && (inputBuffer[1] == '\0'
                                     || inputBuffer[1] == ' ') )
      {
         if (inputBuffer[1] == '\0')
            inputBuffer[2] = '\0';
         
         okToAdd = findCommandInHistory(inputBuffer[2], tempString);

         if ( okToAdd )
         {
            strncpy (inputBuffer, tempString.c_str(), MAX_LINE);
           
         }
         else
         {
            cout << " Command not found\n";
            cout.flush();
         }
      }

      // Create the child and have it run the process
      if((pid = fork()) < 0)
      {
         cerr <<"*** ERROR: forking failed\n";
         exit(1);
      }
      else if (pid == 0) // child process
      {
         // if its in the background 
         if (*background){
            fclose(stdin);
            fopen("/dev/null", "r");
            execvp(inputBuffer,args);
            okToAdd = false;
            cerr << "command not found \n";
            exit(1);
            
         }else{
            execvp(inputBuffer,args);
            okToAdd = false;
            cerr << "command not found \n";
            exit(1);
         }
         
      }
      else // parent process
      {
         
         if( *background)
         {
            //dont wait if its a background process
         }else{
            waitpid(pid,&status,0);
         }
      
      
      }
      
      // add command to history
      for ( int i = 0; i < ct; i++)
      {
         temp += (string) args[i];
         if ( i != (ct - 1) )
            temp += " ";
      }
      if( okToAdd )
      {
         add2History(temp);
      }
      // cout << "history: " << displayHistory();
      args[ct] = NULL; /* just in case the input line was > 80 */
   }
  
   signaled = false;
} 

/**********************************************************************
 *      Main will initialize some variables and go into setup which
 * will get commands and use fork and exec to run them in the shell.
 * the program also handles signals to display history and quit.
 *********************************************************************/
int main(void)
{
   char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
   int background;             /* equals 1 if a command is followed by '&' */
   char *args[MAX_LINE/2];   /* command line (of 80) has max of 40 arguments */
   struct sigaction handler;
   handler.sa_handler = handle_SIGQUIT;
   sigaction(SIGQUIT, &handler, NULL);
   //strncpy( msgHandler, displayHistory(), MAX_LINE );
   while (1)                   /* Program terminates normally inside setup */
   {
      background = 0;
      printf(" COMMAND-> ");
      fflush(stdout);
      setup(inputBuffer, args, &background);       /* get next command */

   }
}


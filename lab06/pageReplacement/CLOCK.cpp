/***********************************************************************
* Program:
*    Lab PageReplacemenlT2t
*    Brother Jones, CS 345
* Author:
*    Winson So
* Summary:
*    This is a program which is using the Clock replacement 
*	 algorithms to replace pages.
************************************************************************/
/*
**====================================================================**
* The following will be filled out for the second submission.  For the
* first submission, you may leave it in your code or delete it.
*
* The second submission should be submitted to labPageReplacementT2.
* (Which means the third line of the headers in CLOCK.cpp, FIFO.cpp and
* LRU.cpp files need to have a "T2" added onto the name of the lab.)
**====================================================================**
*
* Changes made to my code for the re-submission:
*   (examples:)
*   -I added comments such that they would help me remember what I did to
*    write this program in another six months.
*   -Fixed an error, ...
*   - ...
*
************************************************************************/

#include "vmalgorithm.h"

/**************************************************************************
 * Function:
 *    void CLOCK::execute() - Executes the CLOCK page replacement algorithm
 * Inherited Variables and Methods:
 *    int NUM_FRAMES: How many frames there are in 'memory'. (eg. 4)
 *    int frames[NUM_FRAMES]: The array that contains the pages resident
 *          in memory.  Use this array for the pages in memory.  
 *          See Also: NUM_FRAMES
 *    int hit(int page): Function that scans the 'frames[]' array, searching
 *          for 'page'.  If 'page' is found, it will return the index of 
 *          the 'frames[]' array.  If the page is not found, the function
 *          returns '-1'.
 *    int myPageFactory->getPage(): Returns the next page the psuedo-program 
 *          requests.  When there are no more pages to return, this function
 *          returns a -1.
 *    void display(int page, int frames[], bool fault): Function to display
 *          the information to stdout.  The parameter 'page' is the current
 *          page (the one returned from myPageFactory->getPage()), 'frames[]'
 *          is the array of the current resident set, and when 'fault' is 
 *          true, the display function will indicate it to the screen. 
 *          This function must be called for every page that is accessed.
 *************************************************************************/ 
void CLOCK::execute()
{
   bool secondChance[NUM_FRAMES]; 
   for (int i = 0; i < NUM_FRAMES; ++i) {
      // empty cells 
      secondChance[i] = false;
   }

   int page;     
   int next = 0; 

   while ( (page = myPageFactory->getPage()) != -1 )
   {
      int hitIndex = hit(page);

      bool fault = hitIndex == -1;
      if (fault)
      {

         // find next cell that doesn't have a second chance
         // taking away second chances as we go
         while (secondChance[next] == true) {
            secondChance[next] = false;
            next = (next + 1) % NUM_FRAMES;
         }

         // put in the page and give it a second chance
         frames[next] = page;
         secondChance[next] = true;
         next = (next + 1) % NUM_FRAMES;
      } else {

         // if it's accessed it deserves a second chance
         secondChance[hitIndex] = true;
      }

      display(page, frames, fault);
   }
}


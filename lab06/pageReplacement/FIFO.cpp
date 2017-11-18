/***********************************************************************
* Program:
*    Lab PageReplacemenT2t
*    Brother Jones, CS 345
* Author:
*    Winson So
* Summary:
*    This is a program which is using the First In First Out replacement 
*	 algorithms to replace pages.
************************************************************************/

#include "vmalgorithm.h"

/**************************************************************************
 * Function:
 *    void FIFO::execute() - Executes the FIFO page replacement algorithm
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
void FIFO::execute()
{
   bool fault;    // Keep track of faults,
   int page;      // Hold the accessed page
   int last = 0;  // point to the oldest page

   while ( (page = myPageFactory->getPage()) != -1 )
   {
      fault = hit(page) == -1;
      if (fault)
      {
         frames[last] = page;
         last = (last + 1) % NUM_FRAMES;
      } 

      display(page, frames, fault);
   }
}


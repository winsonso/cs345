/***********************************************************************
* Program:
*    Lab PageReplacemenlT2
*    Brother Jones, CS 345
* Author:
*    Winson So
* Summary:
*    This is a program which is using the Least Recently Used replacement 
*	 algorithms to replace pages.
************************************************************************/

#include "vmalgorithm.h"

/**************************************************************************
 * Function:
 *    void LRU::execute() - Executes the LRU page replacement algorithm
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
void LRU::execute()
{
   bool fault;   // Keep track of faults,
   int page;     // Hold the accessed page
   int timestamp[NUM_FRAMES]; // how frequently the page has been accessed
   unsigned long long time = 0;

   for (int i = 0; i < NUM_FRAMES; ++i) {
      timestamp[i] = time;
   }

   while ( (page = myPageFactory->getPage()) != -1 )
   {
      time++;

      int hitIndex = hit(page);

      fault = (hitIndex == -1);
      if (fault)
      {
         int min = 0;

         // find least recently used page
         for (int i = 0; i < NUM_FRAMES; ++i) {
            // update min if we find a earlier timestamp
            min = timestamp[min] <= timestamp[i] ? min : i;
         }

         // replace LRU page
         frames[min] = page;

         // update timestamp
         timestamp[min] = time;
      } 
      else 
      {
         // update timestamp
         timestamp[hitIndex] = time;
      }

      display(page, frames, fault);
   }
}


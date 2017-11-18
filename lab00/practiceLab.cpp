/***********************************************************************
* Program:
*    Lab 00, Practice Lab Submission
*    Brother Jones, CS345
* Author:
*    Winson So
* Summary:
*    The program is to count the number of integers entered by the user
*    at the command line before the user enters a 0
************************************************************************/

#include <iostream>
using namespace std;

/**********************************************************************
 * This function will ask the user to input a number. It will count
 * the number of times the user input a number, it then return the result.
 ***********************************************************************/
int main()
{
   long answer;
   int counter = -1;
   // terminate the program if user inputs 0
   do
   {
      cout << "Enter an integer: ";
      //getline(cin,answer);
      cin >> answer;
      counter++;
   } while (answer != 0);

   // gives the correct output on different conditions.
   if (counter == 1)
   {
      cout << "You entered ";
      cout << counter;
      cout << " integer.\n";
   }
   else if (counter > 1)
   {
      cout << "You entered ";
      cout << counter;
      cout << " integers.\n";
   }
   else
   {
      cout << "No non-zero integers were entered.\n";
   }
   return 0;
}

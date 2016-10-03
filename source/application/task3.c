/*
 * task3.c
 *
 *  Created on: 25/09/2016
 *      Author: ses
 */


/*
 * **************************************************
 * APPLICATION INCLUDE FILES						*
 * **************************************************
 */
#include "task3.h"


/*
 * **************************************************
 * LOCAL DECLARATIONS								*
 * **************************************************
 */



/*
 * **************************************************
 * GLOBAL VARIABLES (extern)						*
 * **************************************************
 */



/*
 * **************************************************
 * FILE SCOPE VARIABLES (static)					*
 * **************************************************
 */
timing_T tiClock3;


/*
 * **************************************************
 * LOCAL PROTOTYPES									*
 * **************************************************
 */



/*
 * **************************************************
 * PUBLIC FUNCTIONS									*
 * **************************************************
 */
/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      INITTASK3()
*      Initialize task1.
*
*  Inputs:
*
*  Outputs:
*
*  Author: Ehsan Shafiei
*  		   Sep 2016
*  -------------------------------------------------------  *
*/
void InitTask3(void)
{
	task3.period = 11;

	tiClock3.clockTickL = 0ul;
	tiClock3.clockTickH = 0ul;

} // END: InitTask3()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      EXECTASK3()
*      Execute the next step of task3.
*
*  Inputs:
*
*  Outputs:
*
*  Author: Ehsan Shafiei
*		   Sep 2016
*  -------------------------------------------------------  *
*/
void ExecTask3(void)
{
	static real32_T count = 0;

	count++;
	if (count == task3.period / baseTs)
	{
		printf("task3 has run for %d seconds.\n", task3.period);
		fflush(stdout);
		count = 0;
	}

	tiClock3.clockTickL++;
	if (!tiClock3.clockTickL)
	{
		tiClock3.clockTickH++;
	}

} // END: ExecTask3()


/*
 * **************************************************
 * LOCAL FUNCTIONS									*
 * **************************************************
 */
/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      PUBLICFUCTION()
*      What this function is doing.
*
*  Inputs:
*      x : Input
*
*  Outputs:
*      y : Return 0 when succeeded.
*
*  Author: Ehsan Shafiei
*  		   Aug 2016
*  -------------------------------------------------------  *
*/
//static type LocalFunction(type x)
//{
//    return y;
//} // END: LocalFunction()

// EOF: task3.c

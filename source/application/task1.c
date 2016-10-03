/*
 * task1.c
 *
 *  Created on: 25/09/2016
 *      Author: ses
 */


/*
 * **************************************************
 * APPLICATION INCLUDE FILES						*
 * **************************************************
 */
#include "task1.h"


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
timing_T tiClock1;


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
*      INITTASK1()
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
void InitTask1(void)
{
	task1.period = 4;

	tiClock1.clockTickL = 0ul;
	tiClock1.clockTickH = 0ul;

} // END: InitTask1()


/**
*  -------------------------------------------------------  *
*  FUNCTION:
*      EXECTASK1()
*      Execute the next step of task1.
*
*  Inputs:
*
*  Outputs:
*
*  Author: Ehsan Shafiei
*		   Sep 2016
*  -------------------------------------------------------  *
*/
void ExecTask1(void)
{
	static real32_T count = 0;

	count++;
	if (count == task1.period / baseTs)
	{
		printf("task1 has run for %d seconds.\n", task1.period);
		fflush(stdout);
		count = 0;
	}

	tiClock1.clockTickL++;
	if (!tiClock1.clockTickL)
	{
		tiClock1.clockTickH++;
	}

} // END: ExecTask1()


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

// EOF: task1.c

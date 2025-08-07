/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        timer.c
*
* @author    trongkn
* 
* @version   1.0
* 
* @date      
* 
* @brief     
*
*******************************************************************************
*
* Detailed Description of the file. If not used, remove the separator above.
*
******************************************************************************/

#include <timer.h>

/******************************************************************************
* External objects
******************************************************************************/
#define TIMER_1	            (unsigned short)0x0001
#define TIMER_2	            (unsigned short)0x0002
#define TIMER_3	            (unsigned short)0x0004
#define TIMER_CMT0_VALUE    (383)

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/



/******************************************************************************
* Local types
******************************************************************************/
/* Timer record for each timer */
typedef struct Timer
{
	uint16_t TimerId;
	uint32_t Period;
	uint32_t Max_cnt;
	bool     one_time;
	struct Timer *pNext;
}tTimer;

static tTimer *mpTimerList = NULL;

/* Global variable for reference */
static unsigned short gTimer;

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/



/******************************************************************************
* Local functions
******************************************************************************/
static tTimer * TIMER_FindTimer(uint16_t TimerId)
{
	tTimer *pTimer;

    /* Move to the start of the list */
	pTimer = mpTimerList;

    /* Check if list is empty */
	if(pTimer != NULL)
    {
        /* Move through the list until the timer id is found */
    	while((pTimer->pNext != NULL) && (pTimer->TimerId != TimerId))
        {
            /* Timer ids - not match, Move onto the next timer */
        	pTimer = pTimer->pNext;
        }
    }
	return (pTimer);
}



/******************************************************************************
* Global functions
******************************************************************************/


//if(pNewTimer != NULL)
//{
//pNewTimer->TimerId = TimerId;
//pNewTimer->pNext = NULL;
//
///* Check if the list is empty */
//if(pTimer == NULL)
//{
///* Store the address of this timer as a first element in the list */
//mpTimerList = pNewTimer;
//}
//else
//{
///* Add the new timer to the end of the list */
//pTimer->pNext = pNewTimer;
//}
//}

PUBLIC bool TIMER_AddTimerMs(uint16_t TimerId, uint32_t Period,bool onetime)
{
	tTimer *pTimer;
	tTimer *pNewTimer = NULL;
	bool ReturnValue = FALSE;
	uint32_t count = 0;
    /* Look for the timer – if already exists */
	pTimer = TIMER_FindTimer(TimerId);

    /* Check if the timer was found */
	if((pTimer == NULL) || (pTimer->TimerId != TimerId))
    {
        /* Create a new timer */
    	pNewTimer = malloc(sizeof(tTimer));

    	if(pNewTimer != NULL)
        {
    		pNewTimer->one_time = onetime;
        	pNewTimer->TimerId = TimerId;
        	pNewTimer->pNext = NULL;

            /* Check if the list is empty */
        	if(pTimer == NULL)
            {
                /* Store the address of this timer as a first element in the list */
            	mpTimerList = pNewTimer;
            }
        	else
            {
                /* Add the new timer to the end of the list */
            	pTimer->pNext = pNewTimer;
            }
        }

        /* Select the new timer */
    	pTimer = pNewTimer;
    }

	if(pTimer != NULL)
    {
		count = (uint32_t)((Period *1000)/TIMER_CMT0_VALUE);
        /* Set the timer interval */
    	pTimer->Period = count;
    	pTimer->Max_cnt = count;
    	ReturnValue = TRUE;
    }

	return ReturnValue;
}

PUBLIC void TIMER_DeteteTimer(uint16_t TimerId)
{
	tTimer *pTimer;

    /* Look for the timer – if already exists */
	pTimer = TIMER_FindTimer(TimerId);

    /* Check if the timer was found */
	while((pTimer != NULL))
    {
        /* delete timer */
    	 free(pTimer);
    	 free(mpTimerList);
		memset(pTimer,0,sizeof(tTimer));
    	 pTimer = NULL;
    }


}
void TIMER_SystemTickEvent(void)
{
	tTimer *pTimer;

    /* Update the timers */
	pTimer = mpTimerList;

	while(pTimer != NULL)
    {
    	if(pTimer->Period != 0)
        {
            /* Decrement the timer period */
        	pTimer->Period--;

            /* Check if the timer has reached zero */
        	if(pTimer->Period == 0)
            {
                /* Set the corresponding bit when the timer reaches zero */
            	gTimer = gTimer | pTimer->TimerId;
            	pTimer->Period = (pTimer->one_time)?0:pTimer->Max_cnt;
            }
        }
        /* Move to the next timer in the list */
    	pTimer = pTimer->pNext;
    }
}

/**
 * @brief One line documentation 
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */

PUBLIC void delay_clk (uint16_t delay)
{	
	uint16_t time = delay;
	uint16_t i = 0; 
	//while (time--!=0);
	for(i=0;i<time;i++)
	{
		
	}
}

void TIMER_CheckTimerEvent()
{
	unsigned short nTimer;

    /* Read the global variable gTimer and reset the value */
	nTimer = gTimer;
	gTimer = 0;


    /* Check for TimerId */
	if( nTimer != 0)
    {
    	if(nTimer & TIMER_1)
        {
    	//	UART_Debug("timer1\r\n");
            /* Call Timer 1 routine */
        //	Timer1Function();
    		ADC_UpdateTdsDisplay();
        }
    	if(nTimer & TIMER_2)
        {
            /* Call Timer 2 routine */
        //	Timer2Function();
    	//	UART_Debug("timer2\r\n");
//    		UART_CheckDataReadDonePacket();
    	//	TIMER_DeteteTimer(TIMER_1);
        }

    }
}


PUBLIC void TIMER_Init()
{

	TIMER_AddTimerMs(TIMER_2,2,0);
	TIMER_AddTimerMs(TIMER_1, 5000,0);

}
/**
 * @brief One line documentation 
 * interrup every 1 ms
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */

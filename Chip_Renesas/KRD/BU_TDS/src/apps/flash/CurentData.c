/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        CurentData.c
*
* @author    quanvu
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


#include "CurentData.h"
#include "flash_app.h"
#include <string.h>

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
static const SavedData DATA_DEFAULD = {
		.tdsOutStore = 0,
};
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
SavedData curentData;


/******************************************************************************
* Local functions
******************************************************************************/
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


void curentData_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)&curentData, CURENT_DATA_BLOCK, sizeof(curentData));
}

/******************************************************************************
* Global functions
******************************************************************************/

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
void curentData_init()
{
    bool readOk = flash_app_readData((uint8_t*)&curentData,CURENT_DATA_BLOCK,sizeof(curentData));

    if(!readOk)
    {
    	memcpy(&curentData,&DATA_DEFAULD,sizeof(DATA_DEFAULD));
    	curentData_updateToFlash();
    }

}
// call when update tds display
void curentData_updateTdsStore(uint16_t tdsOut)
{
	if(((tdsOut +5) < curentData.tdsOutStore) || ((curentData.tdsOutStore +5) < tdsOut))
	{
		curentData.tdsOutStore = tdsOut;
		curentData_updateToFlash();
	}
}

void curentData_resetData()
{
	memcpy(&curentData,&DATA_DEFAULD,sizeof(DATA_DEFAULD));
	curentData_updateToFlash();
}

uint16_t curentData_getLastTdsOut()
{
	return curentData.tdsOutStore;
}

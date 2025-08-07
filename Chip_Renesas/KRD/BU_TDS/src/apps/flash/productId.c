/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2019.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        productId.c
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


#include "productId.h"
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
static const productId_t DATA_DEFAULD = {
		.id = "MKP_DV",
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
productId_t s_productId;


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


void productId_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)&s_productId, PRODUCT_ID_DATA_BLOCK, sizeof(s_productId));
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
void productId_init()
{
    bool readOk = flash_app_readData((uint8_t*)&s_productId,PRODUCT_ID_DATA_BLOCK,sizeof(s_productId));

    if(!readOk)
    {
    	memcpy(&s_productId,&DATA_DEFAULD,sizeof(DATA_DEFAULD));
    	productId_updateToFlash();
    }

}
// call when update tds display
bool productId_setId(char *newId)
{
	if(strlen(newId) < MAX_LEN_ID)
	{
		strcpy(s_productId.id,newId);
		productId_updateToFlash();
		return true;
	}
	else
	{
		return false;
	}
}

void productId_getId(char *currentId)
{
	strcpy(currentId,s_productId.id);
}



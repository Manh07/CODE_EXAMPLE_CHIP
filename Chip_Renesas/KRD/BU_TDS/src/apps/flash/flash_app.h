/*
 * flash_app.h
 *
 *  Created on: Jul 27, 2018
 *      Author: Admin
 */

#ifndef APPS_FLASH_FLASH_APP_H_
#define APPS_FLASH_FLASH_APP_H_

/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         flash_app.h
 *
 * @author    	quanvu
 *
 * @version   1.0
 *
 * @date
 *
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include "r_flash_rx_if.h"
#include "r_flash_rx_config.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/
#define FILLTER_TIME_BLOCK 		FLASH_DF_BLOCK_0
#define USER_CONFIG_BLOCK 		FLASH_DF_BLOCK_1
#define TDS_PARAM_BLOCK     	FLASH_DF_BLOCK_2
#define FILLTER_EXPIRE_BLOCK 	FLASH_DF_BLOCK_3
#define CURENT_DATA_BLOCK 		FLASH_DF_BLOCK_4
#define PRODUCT_ID_DATA_BLOCK 		FLASH_DF_BLOCK_5

#define MAX_LEN_FLASH       (1024) //1Kb
#define BYTE_SAVE_LEN      (2)
#define NUMBER_BYTE_VALID_MAX  (MAX_LEN_FLASH - BYTE_SAVE_LEN)
/******************************************************************************
* Types
******************************************************************************/
union data_len
{
	uint16_t len;
	uint8_t  len_byte[BYTE_SAVE_LEN];
};

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void flash_app_init();
void flash_app_eraseBlock(flash_block_address_t blockAdress);
bool flash_app_writeBlock(uint8_t * data, flash_block_address_t blockAdress,uint16_t dataSize);
bool flash_app_readData(uint8* dataRead, flash_block_address_t blockAdress,uint16_t dataSize);
void flash_app_factoryReset();

/******************************************************************************
* Inline functions
******************************************************************************/






#endif /* APPS_FLASH_FLASH_APP_H_ */

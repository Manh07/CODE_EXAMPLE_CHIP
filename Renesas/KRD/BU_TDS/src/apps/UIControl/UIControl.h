/*
 * UIControl.h
 *
 *  Created on: Jul 28, 2018
 *      Author: Admin
 */
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         UIControl.h
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

#ifndef APPS_UICONTROL_UICONTROL_H_
#define APPS_UICONTROL_UICONTROL_H_

/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>


/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/
#define VOICE_CMD_GET_HOT 1
#define VOICE_CMD_GET_COLD 2
#define VOICE_CMD_GET_RO 3
#define VOICE_CMD_STOP 4


/******************************************************************************

* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

void UIControl_setUIVoiceCommand(uint16_t cmd);
void UIControl_setUIVoiceCommandOnline(uint16_t cmd, uint16_t amount, uint8_t tem);
void UIControl_setUiVoiceCommandCancelWater();
void UIControl_process();
/******************************************************************************
* Inline functions
******************************************************************************/




#endif /* APPS_UICONTROL_UICONTROL_H_ */

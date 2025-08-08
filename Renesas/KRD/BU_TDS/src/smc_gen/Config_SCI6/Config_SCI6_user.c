/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_SCI6_user.c
* Component Version: 1.11.0
* Device(s)        : R5F51303AxFM
* Description      : This file implements device driver for Config_SCI6.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_SCI6.h"
/* Start user code for include. Do not edit comment generated here */
#include "queue.h"
#include "uart.h"
#include "stdint.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci6_tx_address;                /* SCI6 transmit buffer address */
extern volatile uint16_t  g_sci6_tx_count;                   /* SCI6 transmit data number */
extern volatile uint8_t * gp_sci6_rx_address;                /* SCI6 receive buffer address */
extern volatile uint16_t  g_sci6_rx_count;                   /* SCI6 receive data number */
extern volatile uint16_t  g_sci6_rx_length;                  /* SCI6 receive data length */
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t g_rx_char;
/* Flag used to control transmission to PC terminal */
volatile uint8_t g_tx_flag = 0U;
/* Flag used to detect whether data is received */
volatile uint8_t g_rx_flag;
/* Flag used to detect completion of transmission */
static volatile uint8_t SCI6_txdone;

static QUEUE_NODE_T* s_uart_data_queue ;

LOCAL volatile uint16_t  s_rev_index = 0;

LOCAL  uint16_t  s_rev_index_pre = 0;

LOCAL volatile bool  s_rev_done = false;

volatile uint8_t cntTimeRev = 10;
MD_STATUS R_SCI6_AsyncTransmit(uint8_t * const tx_buf, const uint16_t tx_num);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI6_Create_UserInit
* Description  : This function adds user code after initializing the SCI6 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI6_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
	s_uart_data_queue =  QUEUE_InitQueue(MAX_QUEUE_DATA_UART,sizeof(uint8_t));
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_transmit_interrupt
* Description  : This function is TXI6 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI6_TXI6
#pragma interrupt r_Config_SCI6_transmit_interrupt(vect=VECT(SCI6,TXI6),fint)
#else
#pragma interrupt r_Config_SCI6_transmit_interrupt(vect=VECT(SCI6,TXI6))
#endif
static void r_Config_SCI6_transmit_interrupt(void)
{
    if (0U < g_sci6_tx_count)
    {
        SCI6.TDR = *gp_sci6_tx_address;
        gp_sci6_tx_address++;
        g_sci6_tx_count--;
    }
    else
    {
        SCI6.SCR.BIT.TIE = 0U;
        SCI6.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_transmitend_interrupt
* Description  : This function is TEI6 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI6_TEI6
#pragma interrupt r_Config_SCI6_transmitend_interrupt(vect=VECT(SCI6,TEI6),fint)
#else
#pragma interrupt r_Config_SCI6_transmitend_interrupt(vect=VECT(SCI6,TEI6))
#endif
static void r_Config_SCI6_transmitend_interrupt(void)
{
    /* Set TXD6 pin */
    PORTB.PMR.BYTE &= 0xFDU;

    SCI6.SCR.BIT.TIE = 0U;
    SCI6.SCR.BIT.TE = 0U;
    SCI6.SCR.BIT.TEIE = 0U;

    r_Config_SCI6_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_receive_interrupt
* Description  : This function is RXI6 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI6_RXI6
#pragma interrupt r_Config_SCI6_receive_interrupt(vect=VECT(SCI6,RXI6),fint)
#else
#pragma interrupt r_Config_SCI6_receive_interrupt(vect=VECT(SCI6,RXI6))
#endif
static void r_Config_SCI6_receive_interrupt(void)
{
    if (g_sci6_rx_length > g_sci6_rx_count)
    {
        *gp_sci6_rx_address = SCI6.RDR;
        gp_sci6_rx_address++;
        g_sci6_rx_count++;
    }

    if (g_sci6_rx_length <= g_sci6_rx_count)
    {
        /* All data received */
        SCI6.SCR.BIT.RIE = 0U;
        SCI6.SCR.BIT.RE = 0U;
        r_Config_SCI6_callback_receiveend();
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_receiveerror_interrupt
* Description  : This function is ERI6 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI6_ERI6
#pragma interrupt r_Config_SCI6_receiveerror_interrupt(vect=VECT(SCI6,ERI6),fint)
#else
#pragma interrupt r_Config_SCI6_receiveerror_interrupt(vect=VECT(SCI6,ERI6))
#endif
static void r_Config_SCI6_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_Config_SCI6_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI6.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI6.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_callback_transmitend
* Description  : This function is a callback function when SCI6 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI6_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI6_callback_transmitend. Do not edit comment generated here */
	SCI6_txdone = 1U;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_callback_receiveend
* Description  : This function is a callback function when SCI6 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI6_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI6_callback_receiveend. Do not edit comment generated here */
	g_rx_flag = 1U;
	R_Config_SCI6_Serial_Receive((uint8_t *)&g_rx_char, 1);
	QUEUE_EnQueue(s_uart_data_queue,(void*)&g_rx_char);
//	s_rev_index_pre = s_rev_index;
	s_rev_index = s_rev_index+1;
	UART_checkEnShowStart(g_rx_char);
	cntTimeRev = 10;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI6_callback_receiveerror
* Description  : This function is a callback function when SCI6 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI6_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI6_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
MD_STATUS R_SCI6_AsyncTransmit (uint8_t * const tx_buf, const uint16_t tx_num)
{
	 MD_STATUS status = MD_OK;

	 /* Clear transmit completion flag before transmission */
	 SCI6_txdone = 0U;
	/* Set SCI2 transmit buffer address and start transmission */
	 status = R_Config_SCI6_Serial_Send(tx_buf, tx_num);
	 /* Wait for transmit end flag */
	 while (0U == SCI6_txdone)
	 {
	 /* Wait */
	 }
	 return (status);
}

/**********************************************************************
*********
* End of function R_SCI6_AsyncTransmit
*****************************************************************
**************/

PUBLIC uint16_t  UART_ReadData(char * data,uint16_t maxlen)
{
	uint16_t ret = 0;
	if(data  == NULL)
	{
	 return ret;
	}

	while(!QUEUE_QueueIsEmpty(s_uart_data_queue))
	{
	    QUEUE_DeQueue(s_uart_data_queue,data+ret);
		ret ++;
		if(ret > maxlen) return ret;
	}
	s_rev_index = 0;
	s_rev_index_pre = 0;
	s_rev_done = false;
	g_rx_flag = 0U;
	return ret;
}

// timer 1 ms check if
PUBLIC void UART_CheckDataReadDonePacket (void )
{
	if((s_rev_index == s_rev_index_pre) && (g_rx_flag))
	{
		s_rev_done = true;
	}
	s_rev_index_pre = s_rev_index;
}


PUBLIC bool UART_IsDoneFrame(void)
{
	if(cntTimeRev)
	{
		cntTimeRev--;
		if(cntTimeRev == 0)
		{
			return true;
		}
	}
	return false;
//	return s_rev_done;
}

extern bool g_beforemain;
bool UART_checkEnShowStart(uint8_t rxChar)
{
	if(!g_beforemain) return 1;
	static uint8_t cntCheck = 0;
	if(rxChar == 'a')
	{
		cntCheck++;
		if(cntCheck == 5)
		{
			cntCheck = 0;
			//reset
			s_rev_index = 0;
			s_rev_index_pre = 0;
			s_rev_done = false;
			g_rx_flag = 0U;
			QUEUE_CleanQueue(s_uart_data_queue);
		}
	}
	return 1;

}
/* End user code. Do not edit comment generated here */

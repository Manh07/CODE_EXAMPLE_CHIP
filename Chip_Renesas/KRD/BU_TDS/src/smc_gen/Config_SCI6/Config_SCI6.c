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
* File Name        : Config_SCI6.c
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
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_sci6_tx_address;                /* SCI6 transmit buffer address */
volatile uint16_t  g_sci6_tx_count;                   /* SCI6 transmit data number */
volatile uint8_t * gp_sci6_rx_address;                /* SCI6 receive buffer address */
volatile uint16_t  g_sci6_rx_count;                   /* SCI6 receive data number */
volatile uint16_t  g_sci6_rx_length;                  /* SCI6 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI6_Create
* Description  : This function initializes the SCI6 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI6_Create(void)
{
    /* Cancel SCI stop state */
    MSTP(SCI6) = 0U;

    /* Set interrupt priority */
    IPR(SCI6, TXI6) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI6.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI6.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI6.SIMR1.BIT.IICM = 0U;
    SCI6.SPMR.BYTE = _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI6.SMR.BYTE = _02_SCI_CLOCK_PCLK_16 | _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_STOP_1 | 
                    _00_SCI_PARITY_DISABLE | _00_SCI_DATA_LENGTH_8 | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
    SCI6.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI6.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE | _10_SCI_8_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | 
                     _00_SCI_BAUDRATE_SINGLE | _00_SCI_LOW_LEVEL_START_BIT;

    /* Set bit rate */
    SCI6.BRR = 0x0CU;

    /* Set RXD6 pin */
    MPC.PB0PFS.BYTE = 0x0BU;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set TXD6 pin */
    MPC.PB1PFS.BYTE = 0x0BU;
    PORTB.PODR.BYTE |= 0x02U;
    PORTB.PDR.BYTE |= 0x02U;

    R_Config_SCI6_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI6_Start
* Description  : This function starts the SCI6 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI6_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI6, TXI6) = 0U;
    IR(SCI6, RXI6) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI6, TXI6) = 1U;
    IEN(SCI6, TEI6) = 1U;
    IEN(SCI6, RXI6) = 1U;
    IEN(SCI6, ERI6) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI6_Stop
* Description  : This function stops the SCI6 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI6_Stop(void)
{
    /* Set TXD6 pin */
    PORTB.PMR.BYTE &= 0xFDU;

    /* Disable serial transmit */
    SCI6.SCR.BIT.TE = 0U;

    /* Disable serial receive */
    SCI6.SCR.BIT.RE = 0U;

    /* Disable SCI interrupt */
    SCI6.SCR.BIT.TIE = 0U;
    SCI6.SCR.BIT.RIE = 0U;
    IEN(SCI6, TXI6) = 0U;
    IEN(SCI6, TEI6) = 0U;
    IR(SCI6, TXI6) = 0U;
    IEN(SCI6, RXI6) = 0U;
    IR(SCI6, RXI6) = 0U;
    IEN(SCI6, ERI6) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI6_Serial_Receive
* Description  : This function receives SCI6data
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DTC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI6_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > rx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci6_rx_count = 0U;
        g_sci6_rx_length = rx_num;
        gp_sci6_rx_address = rx_buf;
        SCI6.SCR.BIT.RIE = 1U;
        SCI6.SCR.BIT.RE = 1U;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI6_Serial_Send
* Description  : This function transmits SCI6data
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI6_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci6_tx_address = tx_buf;
        g_sci6_tx_count = tx_num;

        /* Set TXD6 pin */
        PORTB.PMR.BYTE |= 0x02U;

        SCI6.SCR.BIT.TIE = 1U;
        SCI6.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

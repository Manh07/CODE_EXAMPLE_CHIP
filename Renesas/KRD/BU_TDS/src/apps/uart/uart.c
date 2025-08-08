/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        uart_drv.c
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


#include <uart.h>
#include "tools.h"
#include "gpio.h"
#include "user_config.h"
#include "adc.h"
#include "productId.h"
#include "uartHandle.h"
#include "heatControl.h"
#include "thermo.h"
#include "Config_SCI6.h"
#include "uartVoice.h"
/******************************************************************************
* External objects
******************************************************************************/

///* Global variable for changing CMT0 interval */
//volatile uint16_t interval_level = 1;
///* String used to print message at PC terminal */
//static char print_str[250];
///* Flag used to detect whether data is received from PC terminal */
//extern volatile uint8_t g_rx_flag;
///* Global variable used for storing data received from PC terminal */
//extern volatile uint8_t g_rx_char;
///* Sends SCI2 data and waits for transmit end flag. */


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
typedef ERR_E(* PACKET_FUNC_PTR)(char* input,char* out );
#define CMD_ID_FOR_TEST_WIFI 21
/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
typedef struct parse_cmd_process_tag
{
    uint8_t					msg_id;
    const char				packet_str[COMMAND_MAX_LEN];
    PACKET_FUNC_PTR			packet_func_ptr;
} PARSE_PACKET_PROCESS_T;

/******************************************************************************
* Local function prototypes
******************************************************************************/
/* Sends SCI2 data and waits for transmit end flag. */
MD_STATUS R_SCI6_AsyncTransmit(uint8_t * const tx_buf, const uint16_t
                               tx_num);

LOCAL ERR_E UART_HandleCalibTds(char* value,char* out );

LOCAL ERR_E UART_HandleSoftVer(char* value,char* out );

LOCAL ERR_E UART_HandleDbg(char* value,char* out );

LOCAL ERR_E UART_HandleTdsParam(char* value,char* out );

LOCAL ERR_E UART_HandleGetAdcTable(char* value,char* out );

LOCAL ERR_E UART_HandleGetTdsTable (char* value,char* out );

LOCAL ERR_E UART_HandleReset (char* value,char* out );

LOCAL ERR_E UART_HandleIoTestEn (char* value,char* out );

LOCAL ERR_E UART_HandleOutSet (char* value,char* out );

LOCAL ERR_E UART_HandleInCheck (char* value,char* out );

LOCAL ERR_E UART_HandleDisableTdsIn (char* value,char* out );

LOCAL ERR_E UART_HandleSentCalibADC (char* value,char* out );

LOCAL ERR_E UART_HandleSentCalibTDS (char* value,char* out );

LOCAL ERR_E UART_HandleSetWaitTimeUpdateTds (char* value,char* out );

LOCAL ERR_E UART_HandleZeroCalib (char* value,char* out );

LOCAL ERR_E UART_HandleGetProductId (char* value,char* out );

LOCAL ERR_E UART_HandleSetProductId (char* value,char* out );

LOCAL ERR_E UART_HandleSetLedBuzz (char* value,char* out );

LOCAL ERR_E UART_HandleHardVer (char* value,char* out );

LOCAL ERR_E UART_HandleCheckHaveWifi (char* value,char* out );


//if model have wifi

LOCAL ERR_E UART_HandleBleWifi (char* value,char* out );
LOCAL ERR_E UART_HandleRegStore (char* value,char* out );
LOCAL ERR_E UART_HandleRegStt (char* value,char* out );
LOCAL ERR_E UART_HandleGetPassword (char* value,char* out );
LOCAL ERR_E UART_HandleGetHwFw (char* value,char* out );
LOCAL ERR_E UART_HandleTestReset (char* value,char* out );
LOCAL ERR_E UART_HandleTestEntryBootLoader (char* value,char* out );
LOCAL ERR_E UART_handleRequestCertificate (char* value,char* out );
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

/******************************************************************************
* Local variables
******************************************************************************/

/* String used to print message at PC terminal */
static char print_str[UART_SEND_MAX_LEN];

/* Global variable used for storing data received from PC terminal */
extern volatile uint8_t g_rx_char;
extern bool g_disableTdsIn;

PUBLIC bool g_ioTestEn = FALSE;
LOCAL bool s_dbg_en = FALSE;
extern volatile uint8_t cntTimeRev;

LOCAL const PARSE_PACKET_PROCESS_T packet_process_table[] =
{


    {1,"CALIB_TDS",UART_HandleCalibTds},
    {2,"MCU_INFO",UART_HandleSoftVer},
    {3,"DEBUG_EN",UART_HandleDbg},
    {4,"TDS_PARAM",UART_HandleTdsParam},
    {5,"RESET",UART_HandleReset},
    {6,"ADC_TABLE",UART_HandleGetAdcTable},
    {7,"TDS_TABLE",UART_HandleGetTdsTable},
    {8,"IOTEST_EN",UART_HandleIoTestEn},
    {9,"OUT_SET",UART_HandleOutSet},
    {10,"IN_CHECK",UART_HandleInCheck},
    {11,"DISABLE_TDS_IN",UART_HandleDisableTdsIn},
    {12,"SENT_ADC",UART_HandleSentCalibADC},
    {13,"SENT_TDS",UART_HandleSentCalibTDS},
    {14,"SET_TIME_UTDS",UART_HandleSetWaitTimeUpdateTds},
    {15,"TDS_ZERO_CLB",UART_HandleZeroCalib},
    {16,"GET_ID",UART_HandleGetProductId},
	{17,"SET_ID",UART_HandleSetProductId},
	{18,"SET_LEDBUZZ", UART_HandleSetLedBuzz},
	{19,"HW_VER", UART_HandleHardVer},
	{20,"HAVE_WIFI", UART_HandleCheckHaveWifi},
	//for wifi: from cmd 21: no reply
	{21,"BLE_WIFI", UART_HandleBleWifi},
	{22,"REG_STORE",UART_HandleRegStore},
	{23,"REG_STT",UART_HandleRegStt},
	{24,"GET_PW",UART_HandleGetPassword},
	{25,"GET_HW_FW",UART_HandleGetHwFw},
	{26,"RQS_RESET",UART_HandleTestReset},
	{27,"RQS_BLD", UART_HandleTestEntryBootLoader},
	{28,"RQS_CERTIFI",UART_handleRequestCertificate}

};

/******************************************************************************
* Local functions
******************************************************************************/

void UART_testReply(uint8_t* data, uint16_t len)
{
	R_SCI6_AsyncTransmit(data, len);
}

LOCAL void UART_Reply(char * data,char* msg_id,ERR_E err_code)
{
    sprintf(print_str,"[%s,%d,%s]",msg_id,err_code,data);
    R_SCI6_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));

}


LOCAL ERR_E UART_HandleSoftVer (char* value,char* out )
{
	sprintf((char*)out,"HW-%d,FW-%d,MD-%d",HARD_VER_NUM,VERSION_SOFTWARE,MODEL_NUM);
#if (MODEL_HAVE_WIFI == 1)
	//send to esp to get hw fw esp
#endif
	return OK;

}

LOCAL ERR_E UART_HandleHardVer (char* value,char* out )
{
	sprintf((char*)out,"%d",HARD_VER_NUM);
    return OK;

}


LOCAL ERR_E UART_HandleDbg (char* value,char* out )
{

    char  param[9] = {0};
    int param_int = 0;
    if(strlen ((char*)value) > 8)
    {

        return ERR_PARAM;
    }

    strcpy((char*)param,(char*)value);
    param_int = atoi((char*)param);
    s_dbg_en = (param_int == 1)?1:0;
    return OK;

}

LOCAL ERR_E UART_HandleIoTestEn (char* value,char* out )
{
    char  param[9] = {0};
    int param_int = 0;
    if(strlen ((char*)value) > 8)
    {

        return ERR_PARAM;
    }

    strcpy((char*)param,(char*)value);
    param_int = atoi((char*)param);
    g_ioTestEn = (param_int == 1)?1:0;
    return OK;
}

LOCAL ERR_E UART_HandleTdsParam (char* value,char* out )
{

    char  param[9] = {0};
    int param_int = 0;
    TDS_E channel_tds = TDS_IN_VALUE;
    if(strlen ((char*)value) > 8)
    {

        return ERR_PARAM;
    }
    //in 0: out 1

    strcpy((char*)param,(char*)value);
    param_int = atoi((char*)param);
    channel_tds =  (param_int == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;
    ADC_GetCalibTdsParam(channel_tds,out);
    return OK;

}

LOCAL ERR_E UART_HandleGetAdcTable (char* value,char* out )
{
    char  param[9] = {0};
    int param_int = 0;
    TDS_E channel_tds = TDS_IN_VALUE;
    if(strlen ((char*)value) > 8)
    {
        return ERR_PARAM;
    }
    //in 0: out 1

    strcpy((char*)param,(char*)value);
    param_int = atoi((char*)param);
    channel_tds =  (param_int == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;
    ADC_GetAdcTable(channel_tds,out);
    return OK;

}
LOCAL ERR_E UART_HandleGetTdsTable (char* value,char* out )
{
    char  param[9] = {0};
    int param_int = 0;
    TDS_E channel_tds = TDS_IN_VALUE;
    if(strlen ((char*)value) > 8)
    {
        return ERR_PARAM;
    }
    //in 0: out 1

    strcpy((char*)param,(char*)value);
    param_int = atoi((char*)param);
    channel_tds =  (param_int == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;
    ADC_GetTdsTable(channel_tds,out);
    return OK;

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
LOCAL ERR_E UART_HandleCalibTds (char* value,char* out )
{
    char data[20] = {0};
    //parser data in uart then handle
    char	*msg_field[MSG_FIELDS_MAX];

    uint8_t index = 0;
    char	*msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;

    // generic indexers
    int 	i;
    int   tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    strcpy(data,value);
    memset( msg_field, 0x00, sizeof(msg_field));
    int channel;
    i				= 0;
    msg_token		= data;
    msg_field[i]	= msg_token;

    while (*msg_token != '\0')
    {


        if ( *msg_token == ':' )
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;


        }
        msg_token++;

    }
    if(i<2) return  ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char*)msg_field[0]);
    channel_tds =  (channel == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;
    index = atoi((char*)msg_field[1]);
    tds   =  atoi((char*)msg_field[2]);
    return	ADC_CalibTdsValueFromUart(tds,channel_tds,index);
//	return OK;

}

LOCAL ERR_E UART_HandleReset (char* value,char* out )
{
    mySoftwareReset();
    return OK;
}

LOCAL ERR_E UART_HandleOutSet (char* value,char* out )
{
    char data[20] = {0};
    //parser data in uart then handle
    char	*msg_field[MSG_FIELDS_MAX];

    char	*msg_token;

    // generic indexers
    int 	i;
    //	len  = MIN(19,strlen_s((char*)value));
    strcpy(data,value);
    memset( msg_field, 0x00, sizeof(msg_field));
    i				= 0;
    msg_token		= data;
    msg_field[i]	= msg_token;

    while (*msg_token != '\0')
    {


        if ( *msg_token == ':' )
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;


        }
        msg_token++;

    }
    if(i<1) return  ERR;
    int output = atoi((char*)msg_field[0]);
    int lever = atoi((char*)msg_field[1]);
    switch (output) {
    case 2:
//        if(lever)
//        {
//            TURN_ON_PUMP;
//        	R_GPIO_PinWrite(PIN_VDT_2,GPIO_LEVEL_HIGH);
//        }
//        else
//        {
//            TURN_OFF_PUMP;
//        	R_GPIO_PinWrite(PIN_VDT_2,GPIO_LEVEL_LOW);
//        }
        break;
    case 1:
//        if(lever)
//        {
//            TURN_ON_VAN_XA;
//        	R_GPIO_PinWrite(PIN_VDT_3,GPIO_LEVEL_HIGH);
//        }
//        else
//        {
//            TURN_OFF_VAN_XA;
//        	R_GPIO_PinWrite(PIN_VDT_3,GPIO_LEVEL_LOW);
//        }
        break;
    default:
        break;
    }
    return	OK;
}

LOCAL ERR_E UART_HandleInCheck (char* value,char* out )
{
    char data[20] = {0};
    //parser data in uart then handle

    char	*msg_field[MSG_FIELDS_MAX];

    char	*msg_token;

    // generic indexers
    int 	i;
    //	len  = MIN(19,strlen_s((char*)value));
    strcpy(data,value);
    memset( msg_field, 0x00, sizeof(msg_field));
    i				= 0;
    msg_token		= data;
    msg_field[i]	= msg_token;

    while (*msg_token != '\0')
    {
        if ( *msg_token == ':' )
        {
            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if(i<1) return  ERR;
    int input = atoi((char*)msg_field[0]);
    int lever = atoi((char*)msg_field[1]);

    ERR_E ret = OK;
    switch (input) {
    case 1: // ap thap
        if(lever && (!CHECK_CO_AP_THAP)) // co ap thap
        {
            ret = INPUT_FALSE;
        }
        else if( !lever && CHECK_CO_AP_THAP) // ko co ap thap
        {
            ret = INPUT_FALSE;
        }
        break;
    case 2: // ap cap
        if(lever && (!CHECK_CO_AP_CAO)) // co ap
        {
            ret = INPUT_FALSE;
        }
        else if( !lever && CHECK_CO_AP_CAO) // ko co ap
        {
            ret = INPUT_FALSE;
        }
        break;
    case 3: // do 1
        sprintf(out, "%d", (ADC_getAdcTdsInDebug() < 0) ? 0 :  ADC_getAdcTdsInDebug());
        break;
    case 4: // do 2
        if(lever && !ADC_GetH2oDet(H2O_2)) // co do
            ret = INPUT_FALSE;
        else if(!lever &&  ADC_GetH2oDet(H2O_2)) // ko do
        {
            ret = INPUT_FALSE;
        }
        break;
    case 5: //tds
		{
			int16_t tds = ADC_getAdcTdsOutDebug();
			sprintf(out, "%d", tds < 0 ? 0 : tds);
		}
    	break;
    default:
        ret = NOT_SUPPORT;
        break;
    }
    return ret;
}
LOCAL void UART_HandleProcess(char * data,uint16_t len)
{

    char	*msg_field[MSG_FIELDS_MAX];
    ERR_E ret = NOT_SUPPORT;
    // pointer used in NMEA message tokenizer
    char	*msg_token;
    char data_reply[UART_SEND_MAX_LEN] = {0};
    // generic indexers
    int 	 j;
    if(*data != MSG_FIELD_SOF) goto end_handle;
    memset( msg_field, 0x00, sizeof(msg_field));

    msg_token		= data+1;
    msg_field[0]	= msg_token;

    while (*msg_token != '\0')
    {

        if (( *msg_token == MSG_FIELD_SEPARATOR ) && (msg_field[1] == 0x00))
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';

            // save position of the next token
            msg_field[1] = msg_token + 1;

        }
        if( *msg_token == MSG_FIELD_EOF)
        {
            *msg_token = '\0';
        }

        msg_token++;

    }

    for( j = 0; j < ARR_SIZE(packet_process_table); j++)
    {

        if(	strcmp((char*) packet_process_table[j].packet_str,(char*) msg_field[MSG_FIELD_TYPE]) == 0)
        {
            ret = packet_process_table[j].packet_func_ptr((char*)msg_field[MSG_FIELD_DATA],(char*)data_reply );
            if(packet_process_table[j].msg_id >= CMD_ID_FOR_TEST_WIFI)
				return;
            else
            	break;
        }

    }



end_handle:
	if(ret == NOT_SUPPORT) return;
    UART_Reply(data_reply,(char*)packet_process_table[j].packet_str,ret);
    return;


}

LOCAL ERR_E UART_HandleDisableTdsIn (char* value,char* out )
{
    char  param[9] = {0};
    int param_int = 0;
    if(strlen ((char*)value) > 8)
    {

        return ERR_PARAM;
    }

    strcpy((char*)param,(char*)value);
    param_int = atoi((char*)param);
    g_disableTdsIn = (param_int == 1)?1:0;
    return OK;
}

LOCAL ERR_E UART_HandleSentCalibADC (char* value,char* out )
{
     char data[150] ;
//		//parser data in uart then handle
    char	*msg_field[(CALIB_POINT_MAX)];

    char	*msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;
//
//		// generic indexers
    int 	i;
    //	len  = MIN(19,strlen_s((char*)value));
    int len = strlen((char*)value);
    strcpy(data,value);
//		strcpy(data,(char*)value);
    memset( msg_field, 0x00, sizeof(msg_field));
    int channel;
    i				= 0;
    msg_token		= data;
    msg_field[i]	= msg_token;

    while (*msg_token != '\0')
    {


        if ( *msg_token == ',' )
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;


        }
        msg_token++;

    }
    if(i<2) return  ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char*)msg_field[0]);
    channel_tds =  (channel == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;

    int  j= 0;

    static int adcTable[CALIB_POINT_MAX];
    for(j = 1; j <= i; j++)
    {
        adcTable[j - 1] = atoi((char*)msg_field[j]);
    }

    return	ADC_WriteAdcFromUart(adcTable,channel_tds );
}


LOCAL ERR_E UART_HandleSentCalibTDS (char* value,char* out )
{
    static char data[150] ;
//		//parser data in uart then handle
    char	*msg_field[(CALIB_POINT_MAX)];

    char	*msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;
//
//		// generic indexers
    int 	i;
    //	len  = MIN(19,strlen_s((char*)value));
    int len = strlen((char*)value);
    memcpy(data,value,len +1);
//		strcpy(data,(char*)value);
    memset( msg_field, 0x00, sizeof(msg_field));
    int channel;
    i				= 0;
    msg_token		= data;
    msg_field[i]	= msg_token;

    while (*msg_token != '\0')
    {


        if ( *msg_token == ',' )
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;


        }
        msg_token++;

    }
    if(i<2) return  ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char*)msg_field[0]);
    channel_tds =  (channel == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;

    int  j= 0;

    static int tdsTable[CALIB_POINT_MAX];
    for(j = 1; j <= i; j++)
    {
        tdsTable[j - 1] = atoi((char*)msg_field[j]);
    }


    return	ADC_WriteTdsFromUart(tdsTable,channel_tds );
}

LOCAL ERR_E UART_HandleSetWaitTimeUpdateTds (char* value,char* out )
{
    char data[20] = {0};
    //parser data in uart then handle
    char	*msg_field[MSG_FIELDS_MAX];

    char	*msg_token;

    strcpy(data,value);
    memset( msg_field, 0x00, sizeof(msg_field));
    msg_token		= data;
    msg_field[0]	= msg_token;
    //0:1:2 mean chanel_in:index1:tds2
    int second = atoi((char*)msg_field[0]);
    if((second < 0) || (second > 3600)) return ERR;
    UserConfig_setWaitTimeUpdateTds(second);

    return OK;

}

LOCAL ERR_E UART_HandleZeroCalib (char* value,char* out )
{
	ADC_CalibTdsAutoZero();
	return OK;
}

LOCAL ERR_E UART_HandleGetProductId (char* value,char* out )
{
#if (MODEL_HAVE_WIFI == 0)
	productId_getId(out);
    return OK;
#else
    //send to esp to esp respond jig
    productId_getId(out);
    uartHandle_getId();
    return ERR_UNKNOWN;
#endif

}

LOCAL ERR_E UART_HandleSetProductId (char* value,char* out )
{
#if (MODEL_HAVE_WIFI == 0)
	if(productId_setId(value))
	{
		return OK;
	}
	else return ERR_PARAM;

#else
	//[SET_ID,RO0025766:<password>]
	char *pId = NULL;
	char *pPw = NULL;
	pId = strtok(value,":");
	if(pId == NULL) return ERR_PARAM;
	pPw = strtok(NULL, ":");
	if(pPw == NULL) return ERR_PARAM;
	if(productId_setId(pId))
	{
		//continue set to esp
		//send to jig from respond of esp
		uartHandle_setId(pId, pPw);
		return ERR_UNKNOWN;
	}
	else
	{
		return ERR;
	}
#endif
}

LOCAL ERR_E UART_HandleSetLedBuzz (char* value,char* out )
{
	char  param[9] = {0};
	int param_int = 0;
	if(strlen ((char*)value) > 8)
	{

		return ERR_PARAM;
	}

	strcpy((char*)param,(char*)value);
	param_int = atoi((char*)param);
//	if(param_int == 1) Buzzer_test();
//	else if(param_int > 1) Led_test(param_int);

	return OK;

}


LOCAL ERR_E UART_HandleCheckHaveWifi (char* value,char* out )
{
	sprintf(out,"%d", MODEL_HAVE_WIFI);
	return OK;
}

LOCAL ERR_E UART_HandleGetHwFw (char* value,char* out )
{
	uartHandle_getHwFwEsp();
	//send to esp to get info
	return ERR_UNKNOWN;
}

LOCAL ERR_E UART_HandleBleWifi (char* value,char* out )
{
	uartHandle_getBleWifi();
	//send to esp to get info
	return ERR_UNKNOWN;
}
LOCAL ERR_E UART_HandleRegStore (char* value,char* out )
{
	uartHandle_setRegStore();
		//send to esp to get info
		return ERR_UNKNOWN;
}
LOCAL ERR_E UART_HandleRegStt (char* value,char* out )
{
	uartHandle_getRegStt();
		//send to esp to get info
		return ERR_UNKNOWN;
}
LOCAL ERR_E UART_HandleGetPassword (char* value,char* out )
{
	uartHandle_getPassWord();
		//send to esp to get info
		return ERR_UNKNOWN;
}
LOCAL ERR_E UART_HandleTestReset (char* value,char* out )
{
	uartHandle_requestReset();
		//send to esp to get info
		return ERR_UNKNOWN;
}
LOCAL ERR_E UART_HandleTestEntryBootLoader (char* value,char* out )
{
	uartHandle_requestBootLoader();
	//send to esp to get info
		return ERR_UNKNOWN;
}
LOCAL ERR_E UART_handleRequestCertificate (char* value,char* out )
{
	uartHandle_requestCertificate();
		//send to esp to get info
		return ERR_UNKNOWN;
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
PUBLIC void UART_Init(void)
{
    //init UART here
//    R_Config_SCI12_Create();
    /* Set SCI2 receive buffer address and enable receive interrupt */
    R_Config_SCI6_Serial_Receive((uint8_t *)&g_rx_char, 1);
    /* Enable SCI2 operation */
    R_Config_SCI6_Start();

    sprintf(print_str, "[MCU_RESET,0,]");
    R_SCI6_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));
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
PUBLIC void UART_UartPuts (char *s)
{
    sprintf(print_str,"%s", s);
    R_SCI6_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));
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
PUBLIC void UART_Debug(char * data)
{
//	#ifdef DEBUG_ENABLE
    if(s_dbg_en)
    {
        UART_UartPuts(data);
    }
//	#endif
}

PUBLIC void UART_Process()
{
    char data[300] = {0};
    uint16_t len = 0;
    if(UART_IsDoneFrame())
    {
        len = UART_ReadData(data,300);
        if(data[0] == MSG_FIELD_SOF && data[len - 1] == MSG_FIELD_EOF)
        {
            if(strstr(data, "[UP,") != NULL)
            {
                //command with thermo module
                //"[UP,%d,%d,%d,%d,%d,%d]",s_tds_out.sma_tds_adc,isEnableCold, temLanh,isEnableHot, temNong,tempColdSetting
                thermo_readThermoValue(data);
            }
            else{
                UART_HandleProcess(data,len);   //test jig
            }
        }
        else
        {
        	uartVoice_processFrameFormat((uint8_t*)data, len);
        }
//        else if(len == 2)
//        {
//        	if(data[0] == CMD_RSP_AA_00_1 && data[1] == CMD_RSP_AA_00_2)
//        	{
//        	}
//        }
//        else if(len == 18)
//        {
//        	if(data[15] == CMD_RSP_A5)
//        	{
//        		heatControl_decodeResponeInquire((uint8_t*)data, len);
//        	}
//        }
//        else {
//
//        }
    }
}

void UART_sendFrame(uint8_t* frame, uint16_t len)
{
	R_SCI6_AsyncTransmit(frame, len);
}

/*
 * uartHandle.c
 *
 *  Created on: Dec 10, 2018
 *      Author: hp
 */
#include "uartHandle.h"
#include "config.h"
#include "Config_SCI1.h"
#include "updateParam.h"
#include "tools.h"
#include "uart.h"
#include "UIControl.h"
#include "getWater.h"
#include "heatControl.h"
#include "voiceCmd.h"
#include "flash_app.h"
#include "UIControl.h"
#define VERSION "version 1.0"
#define MAX_LEN_CMD_NAME (15)
#define MAX_NUM_CMD (18)
#define MAX_NUM_STR (2)

typedef void(*cmd_ptr_function)(uint8_t* , uint8_t* );
typedef struct
{
	uint16_t cmd_id;
	const char cmd_name[MAX_LEN_CMD_NAME];
	cmd_ptr_function ptr_fun;
} command_packet_t;


void uart_handleVersion(uint8_t* data_in, uint8_t* data_out);
void uart_handle_UpdateProperty(uint8_t* data_in, uint8_t* data_out);
void uart_handle_GetIo(uint8_t* data_in, uint8_t* data_out);
void uart_handle_BlinkLed(uint8_t* data_in, uint8_t* data_out);
void uart_handle_UartRespondCmd(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleBleWifi(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleSetId(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleGetId(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleRegStore(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleRegStt(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleGetPassword(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleGetHwFw(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleTestReset(uint8_t* data_in, uint8_t* data_out);
void uart_hanlde_handleTestBootLoader(uint8_t* data_in, uint8_t* data_out);
void uart_handle_handleBleName(uint8_t* data_in, uint8_t* data_out);
void uart_handle_voiceCmd(uint8_t* data_in, uint8_t* data_out);
void uart_handle_voiceCmdOnline(uint8_t* data_in, uint8_t* data_out);
void uart_hanlde_handleTestCertificate(uint8_t* data_in, uint8_t* data_out);
command_packet_t cmd_user[MAX_NUM_CMD] =
{
	{1,"MCU_INFO",uart_handleVersion},
	{2,"UP", uart_handle_UpdateProperty},
	{3,"GET_IO", uart_handle_GetIo},
	{4,"BLINK_LED", uart_handle_BlinkLed},
	{5,"RSP", uart_handle_UartRespondCmd},
	{6,"BLE_WIFI",uart_handle_handleBleWifi},
	{7,"SET_ID",uart_handle_handleSetId},
	{8,"GET_ID",uart_handle_handleGetId},
	{9,"REG_STORE",uart_handle_handleRegStore},
	{10,"REG_STT",uart_handle_handleRegStt},
	{11,"GET_PW",uart_handle_handleGetPassword},
	{12,"GET_HW_FW",uart_handle_handleGetHwFw},
	{13,"RQS_RESET",uart_handle_handleTestReset},		//esp reset
	{14,"RQS_BLD", uart_hanlde_handleTestBootLoader},
	{15,"BLE_NAME", uart_handle_handleBleName},
	{16,"VOICE_CMD", uart_handle_voiceCmd},
	{17,"ONLINE_CMD", uart_handle_voiceCmdOnline},
	{18,"RQS_CERTIFI", uart_hanlde_handleTestCertificate}
};

bool s_mcuIsReset = true;
uart_handle_t uartHandle_struct = {{0},0,0,UART_STT_IDLE};
void uartHandle_Init(void)
{
	//init UART here
//	R_Config_SCI1_Create();
	/* Enable SCI1 operation */
	R_Config_SCI1_Start();
   // printf("hello world\r\n");
}

void uartHandle_sendResetEsp()
{
	if(s_mcuIsReset)
		R_SCI1_AsyncTransmit((uint8_t *)"[MCU_RESET,0,]67", 16);
	s_mcuIsReset = false;
}

void uartHandle_respondOk()
{
	R_SCI1_AsyncTransmit((uint8_t*)"[RSP,OK]7F", 10);
}

//handle command
//data reply
void uartHandle_reply(uint16_t cmd_id, uint8_t* cmd_name,  uint8_t* data_reply)
{
	char buf_reply[100];
	sprintf((char*)buf_reply, "[%s,%s]",cmd_name, data_reply);
	R_SCI1_AsyncTransmit((uint8_t*)buf_reply, strlen(buf_reply));
	//printf("\n[%d, %s, %s]\r\n", cmd_id ,cmd_name, data_reply);
}

void uartHandle_sendString(char* data)
{
	char buf_to_esp[100];
	//add 2 byte cks to trail
	uint8_t cks_H, cks_L, cks;
	cks = tools_calculateChecksum((uint8_t*)data, strlen(data));
	tools_encodeCheckSum(cks, (char*)&cks_H, (char*)&cks_L);
	strcpy(buf_to_esp, data);
	buf_to_esp[strlen(data)] = cks_H;
	buf_to_esp[strlen(data) + 1] = cks_L;
	buf_to_esp[strlen(data) + 2] = '\0';

	R_SCI1_AsyncTransmit((uint8_t*)buf_to_esp, strlen(buf_to_esp));
}

//tach chuoi thanh 2 phan(chuoi): ptr_arr[0] phan ten command, ptr_arr[1]: chuoi tham so
void uartHandle_ProcessData(uint8_t* data_in, uint16_t length)
{
	uint16_t i =0;
	uint8_t data_reply[200] = {0};
	uint8_t* ptr_arr[MAX_NUM_STR] = {NULL, NULL};
	uint8_t num_ptr = 0;
	uint8_t num_cmd =0;
	uint8_t* buf_data_in = NULL;
	buf_data_in = data_in;

		if((buf_data_in[0]== '[') && (buf_data_in[length-1]== ']'))
		{
			ptr_arr[num_ptr] = buf_data_in +1;
			while(i < length )
			{
				if(buf_data_in[i] == ',')
				{

					num_ptr++;
					if(num_ptr == MAX_NUM_STR) break;
					buf_data_in[i] = '\0';
					ptr_arr[num_ptr] = buf_data_in+i+1;
				}
				i++;
			}
				buf_data_in[length-1] = '\0';

			for( num_cmd = 0; num_cmd < MAX_NUM_CMD; num_cmd++)
			{
				if(strcmp((const char*)ptr_arr[0], cmd_user[num_cmd].cmd_name) ==0)
				{
					if((strcmp((const char*)ptr_arr[0], "RSP") != 0) && (strcmp((const char*)ptr_arr[0], "MCU_INFO") != 0) && (strcmp((const char*)ptr_arr[0], "ONLINE_CMD") != 0))
					{
						uartHandle_respondOk();
					}
					cmd_user[num_cmd].ptr_fun(ptr_arr[1], (uint8_t*)data_reply);
//					uartHandle_reply(cmd_user[num_cmd].cmd_id, (uint8_t*)cmd_user[num_cmd].cmd_name,(uint8_t*)data_reply);
					break;
				}
			}

		}
		else
		{
			//R_SCI1_AsyncTransmit((uint8_t*)"command not support",19);
		}
}


uint16_t uartHandle_get_dataFromBuffer(uint8_t* data_out)
{
	//truong hop khung truyen bi nhieu voi cac byte khac
	if(uartHandle_struct.buffer_index < 4) return 0;
	bool haveHdrAndTmn = false;
	uint8_t i ,j;
	for( i = 0; i < uartHandle_struct.buffer_index - 3; i++)
	{
		if(uartHandle_struct.buffer_rx[i] == '[')
		{
			for( j = i + 1; j < uartHandle_struct.buffer_index -2; j++)
			{
				if(uartHandle_struct.buffer_rx[j] == ']')
				{
					haveHdrAndTmn = true;
					break;
				}
			}
			break;
		}

	}
	uint16_t len = 0;
	if(haveHdrAndTmn)
	{
		len = j - i + 1 + 2;	//2 byte cks
		memcpy((void*)data_out,(const void*)(uartHandle_struct.buffer_rx + i), len);
		uartHandle_struct.buffer_index = 0;
	}

    return len;
}

void uartHandle_addDataFromItr(uint8_t u8Data)
{
	if(uartHandle_struct.buffer_index < MAX_SIZE_BUFFER)
	{
		uartHandle_struct.buffer_rx[uartHandle_struct.buffer_index++] = u8Data;
	}
	else{
		uartHandle_struct.buffer_index = 0;
	}
	uartHandle_struct.state_cnt = 3;
}


//tinh lai check sum va so sanh voi 2 byte cuoi
bool uartHandle_checkValidFrame(uint8_t* buf, uint8_t len)
{
	uint8_t cks = tools_calculateChecksum(buf, len - 2);
	uint8_t cks_rsp = tools_decodeCheckSum(buf[len-2], buf[len-1]);
	if (cks != cks_rsp) return false;
	else return true;
}
//ham xu li du lieu ban dau nhan tu uart
void uartHandle_process()
{
    uint8_t data_rx[100] = {0};
    uint16_t length = 0;
		length = uartHandle_get_dataFromBuffer((uint8_t*)data_rx);
		if(length == 0) return;
		if(uartHandle_checkValidFrame((uint8_t*)data_rx, length) == false) {
			//R_SCI1_AsyncTransmit((uint8_t*) data_rx, length);
			return;
		}
		else{
			uartHandle_ProcessData((uint8_t*)data_rx, length - 2);
		}
}

void uartHandle_setTimeCheck(uint8_t time_cnt)
{
	uartHandle_struct.state_cnt = time_cnt;
}
void uartHandle_tick()		//1ms
{
	switch(uartHandle_struct.uart_stt)
	{
		case UART_STT_IDLE:
			if(uartHandle_struct.state_cnt)											//chua co du lieu thi k lam gi ca
			{
				uartHandle_struct.uart_stt = UART_STT_DATA_COMING;
			}
			break;
		case UART_STT_DATA_COMING:
			{
				if(--uartHandle_struct.state_cnt == 0)
					uartHandle_struct.uart_stt = UART_STT_DATA_DONE;//qua time giua 2 lan truyen ki tu
				break;
			}
		case UART_STT_DATA_DONE:

			uartHandle_process();
			uartHandle_struct.uart_stt = UART_STT_IDLE;
			break;

	}
}


//function for cmd_id1
void uart_handleVersion(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50] = "";
	sprintf(buf,"[MCU_INFO,HW-%d,FW-%d,MD-%d]",HARD_VER_NUM,VERSION_SOFTWARE,MODEL_NUM);
	uartHandle_sendString((char*)buf);
}


//function for cmd id2
//ex: [UP,iro_rtt,9,300,300,300,3600,500,300,300,300,400]
//ex2: [UP,iro_rsft,5]
//data_in: iro_rtt,9,300,300,300,3600,500,300,300,300,400
void uart_handle_UpdateProperty(uint8_t* data_in, uint8_t* data_out)
{
	updateParam_handleMsgFromUart(data_in, data_out);

}


void uart_handle_GetIo(uint8_t* data_in, uint8_t* data_out)
{
	updateParam_updateIo();
	strcpy((char*)data_out, (const char*)"OK");
}
bool isWifiDisconnected = true;
typedef enum{
    LED_BLINK_STATE_OFF =       0,
    LED_BLINK_STATE_ON =        1,
    LED_BLINK_STATE_BLINK =     2
}LedBlinkState_t;
void uart_handle_BlinkLed(uint8_t* data_in, uint8_t* data_out)
{
	uint32_t data_wifi[20];
	uint16_t len = 0;
	len = convParamStringToNumber(data_in, data_wifi);
	if( len != 2)
	{
		strcpy((char*)data_out,"error_len");
		return;
	}
    if(data_wifi[0] == LED_BLINK_STATE_ON)
    {
        isWifiDisconnected = false;
    }
    else
    {
        isWifiDisconnected = true;
    }
#ifdef MODEL_AIOTEC_4_3

#endif
}

//[RSP,OK]
extern bool isSendComplete;
void uart_handle_UartRespondCmd(uint8_t* data_in, uint8_t* data_out)
{
	if(strcmp((char*)data_in, "OK") == 0)
	{
		isSendComplete = true;
	}
}

//esp rep:  [BLE_WIFI,%d,]
void uart_handle_handleBleWifi(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50] = "";
	sprintf(buf,"[BLE_WIFI,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
//[SET_ID,0,%s,%s,%s,]",g_product_Id,DEVICE_TYPE_ID,g_password
void uart_handle_handleSetId(uint8_t* data_in, uint8_t* data_out)
{
	char buf[100];
	sprintf(buf,"[SET_ID,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleGetId(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[GET_ID,0,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleRegStore(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[REG_STORE,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleRegStt(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[REG_STT,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleGetPassword(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[GET_PW,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleGetHwFw(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[GET_HW_FW,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleTestReset(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[RQS_RESET,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_hanlde_handleTestBootLoader(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[RQS_BLD,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
//[RQS_CERTIFI,0,]
void uart_hanlde_handleTestCertificate(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[RQS_CERTIFI,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}
void uart_handle_handleBleName(uint8_t* data_in, uint8_t* data_out)
{
	char buf[50];
	sprintf(buf,"[BLE_NAME,%s]",data_in);
	UART_testReply((uint8_t*)buf, strlen(buf));
}

void uart_handle_voiceCmd(uint8_t* data_in, uint8_t* data_out)
{
	uint32_t data_wifi[20];
	uint16_t len = 0;
	len = convParamStringToNumber(data_in, data_wifi);
	if( len != 1)
	{
		strcpy((char*)data_out,"error_len");
		return;
	}
//	UIControl_setUIVoiceCommand(data_wifi[0]);
}
void uart_handle_voiceCmdOnline(uint8_t* data_in, uint8_t* data_out)
{
//	uint16_t data_wifi[20];
//	uint16_t len = 0;
//	len = convParamStringToNumber(data_in, data_wifi);
//	switch(data_wifi[0])
//	{
//	//for online
//		case VOICE_CMD_IDX_GET_WATER_SPECIAL:
//			//get water have tem,amount
//			UIControl_setUIVoiceCommandOnline(data_wifi[1], data_wifi[2],data_wifi[3]);
//			break;
//		case VOICE_CMD_IDX_GET_WATER_UNKNOW:
//			//get water without tem,amount
//			UIControl_setUIVoiceCommandOnline(data_wifi[1], 0,0);
//			break;
//		case VOICE_CMD_IDX_CANCEL_GET_WATER:
//			//stop get water
//			UIControl_setUiVoiceCommandCancelWater();
//			break;
//		case VOICE_CMD_IDX_GET_STATUS:
//			//respond status: busy
//			if(heatControl_waterIsOuting() || getWater_waterIsOuting())
//			{
//				voiceCmd_sendStateWater(WATER_STATE_BUSY);
//			}
//			else
//			{
//				voiceCmd_sendStateWater(WATER_STATE_FREE);
//			}
//			break;
//
//	}
}

void uartHandle_setId(char* id, char* pw)
{
	//[SET_ID,<maCode>,<pw>]
	char buf[80];
	sprintf(buf, "[SET_ID,%s,%s]", id, pw);
	uartHandle_sendString((char*)buf);
}
void uartHandle_getId()
{
	//[GET_ID,1,]
	char buf[20];
	strcpy(buf, "[GET_ID,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_getHwFwEsp()
{
	char buf[20];
	strcpy(buf, "[GET_HW_FW,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_getBleWifi()
{
	char buf[20];
	strcpy(buf, "[BLE_WIFI,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_setRegStore()
{
	char buf[20];
	strcpy(buf, "[REG_STORE,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_getRegStt()
{
	char buf[20];
	strcpy(buf, "[REG_STT,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_getPassWord()
{
	char buf[20];
	strcpy(buf, "[GET_PW,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_requestReset()
{
	char buf[20];
	strcpy(buf, "[RQS_RESET,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_requestBootLoader()
{
	char buf[20];
	strcpy(buf, "[RQS_BLD,1]");
	uartHandle_sendString((char*)buf);
}
void uartHandle_requestCertificate()
{
	char buf[20];
	strcpy(buf, "[RQS_CERTIFI,1]");
	uartHandle_sendString((char*)buf);
}
/*
ex CMD: "3,3,30535,10" --> {3,3,30535,10} , index = 4
*/
#define CHARACTER_DIV ','
#define MAX_PARAM_SIZE 20
uint16_t convParamStringToNumber(uint8_t* dataString_in, uint32_t* dataNumber_out)
{
	uint16_t index = 0; //so phan tu

	char* ptr_token =NULL;
	ptr_token = strtok((char*)dataString_in, ",");
	while(ptr_token != NULL)
	{
		if(index < MAX_PARAM_SIZE)
		{
			dataNumber_out[index++] = (uint32_t)atoi((const char*)ptr_token);
			ptr_token = strtok(NULL, ",");
		}
		else break;
	}
	return index;
}




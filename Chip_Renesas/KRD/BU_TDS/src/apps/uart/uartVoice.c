/*
 * uartVoice.c
 *
 *  Created on: Oct 27, 2022
 *      Author: ASUS
 */


#include "uartVoice.h"
#include "config.h"
#include "uart.h"
#include "getWater.h"
#include "user_config.h"
#include "thermo.h"
#include "updateParam.h"
#include "heatControl.h"
/*change format frame receive and send
 * [<hdr 0x01> <msg code> <lenData> <data_L> < data_H> <cks> <term 0x02>]
 *
 * */
#define MSG_HEADER 0x01
#define MSG_TEMINATE 0x02
#define MAX_LEN_BUFFER_SEND 30
char bufSend[MAX_LEN_BUFFER_SEND];
typedef enum
{
    MSG_CODE_REQUEST_GET_DATA = 0x00,
    MSG_CODE_UP_VAN_XA = 0x10,
    MSG_CODE_UP_BOM = 0x11,
    MSG_CODE_UP_BINH_AP = 0x12,
    MSG_CODE_UP_NUOC_VAO = 0x13,
    MSG_CODE_UP_HSD_REMAIN = 0x14,
    MSG_CODE_UP_HSD_DEFAULT = 0x15,
    MSG_CODE_UP_RUNTIME_DEFAULT = 0x16,
    MSG_CODE_UP_RUNTIME_REMAIN = 0x17,
    MSG_CODE_UP_CP_RESET_FITLER = 0x18,
    MSG_CODE_UP_TDS = 0x19,
    MSG_CODE_UP_TIME_PUMP = 0x1A,
    MSG_CODE_UP_ERROR = 0x1B,
    MSG_CODE_UP_TEMP_HOT = 0x1C,
    MSG_CODE_UP_TEMP_COLD = 0x1D,
    MSG_CODE_UP_CP_SETTING_FASTBLOCK = 0x1E,
    MSG_CODE_UP_CP_ENABLE_HOTTANK = 0x1F,
    MSG_CODE_UP_CP_ENABLE_COLDTANK = 0x20,
    MSG_CODE_UP_CP_GET_WATER = 0x21,
    MSG_CODE_UP_CP_XA_SUC = 0x22,
    MSG_CODE_UP_CP_BU = 0x23,
    MSG_CODE_UP_NOTIFY = 0x24,

} msg_code_t;
#define LEN_DATA_FILTER_TIME_EXPECT FILTER_NUM*2
//luu max code va len expect
uint8_t msgCodeList[MSG_IDX_MAX][2] = {
{     MSG_CODE_REQUEST_GET_DATA,1},
{     MSG_CODE_UP_VAN_XA,1},
{     MSG_CODE_UP_BOM,1},
{     MSG_CODE_UP_BINH_AP,1},
{     MSG_CODE_UP_NUOC_VAO,1},
{     MSG_CODE_UP_HSD_REMAIN,LEN_DATA_FILTER_TIME_EXPECT},
{     MSG_CODE_UP_HSD_DEFAULT,LEN_DATA_FILTER_TIME_EXPECT},
{     MSG_CODE_UP_RUNTIME_DEFAULT,LEN_DATA_FILTER_TIME_EXPECT},
{     MSG_CODE_UP_RUNTIME_REMAIN,LEN_DATA_FILTER_TIME_EXPECT},
{     MSG_CODE_UP_CP_RESET_FITLER,2},
{     MSG_CODE_UP_TDS,2},
{     MSG_CODE_UP_TIME_PUMP,4},
{     MSG_CODE_UP_ERROR,4},
{     MSG_CODE_UP_TEMP_HOT,2},
{     MSG_CODE_UP_TEMP_COLD,2},
{     MSG_CODE_UP_CP_SETTING_FASTBLOCK,3},
{     MSG_CODE_UP_CP_ENABLE_HOTTANK,2},
{     MSG_CODE_UP_CP_ENABLE_COLDTANK,2},
{     MSG_CODE_UP_CP_GET_WATER,3},
{     MSG_CODE_UP_CP_XA_SUC,2},
{     MSG_CODE_UP_CP_BU,2},
{     MSG_CODE_UP_NOTIFY, 2}
};

#define NUM_MSG_UP_CP 7
uint8_t msgIsControlling[NUM_MSG_UP_CP][2] =
{
     {MSG_CODE_UP_CP_RESET_FITLER,false},
     {MSG_CODE_UP_CP_SETTING_FASTBLOCK,false},
     {MSG_CODE_UP_CP_ENABLE_HOTTANK,false},
     {MSG_CODE_UP_CP_ENABLE_COLDTANK,false},
     {MSG_CODE_UP_CP_GET_WATER,false},
     {MSG_CODE_UP_CP_XA_SUC,false},
     {MSG_CODE_UP_CP_BU,false}
};

uint8_t uartVoice_calculateChecksum(uint8_t *buf, uint8_t len)
{
    uint8_t ret = 0;
    uint8_t i = 0;
    for(i = 0; i < len; i++)
    {
        ret += buf[i];
    }
    return ret;
}
void uartVoice_sendFrame(uint8_t codemsg, uint8_t* data, uint16_t len)
{
    uint16_t lenFr = 0;
    bufSend[lenFr++] = MSG_HEADER;
    bufSend[lenFr++] = codemsg;
    bufSend[lenFr++] = len;
    if(data != NULL && len != 0 && (len <= MAX_LEN_BUFFER_SEND -2))
    {
       memcpy(bufSend+lenFr, data, len);
       lenFr +=len;
    }
    uint8_t cks = uartVoice_calculateChecksum((uint8_t*)bufSend, lenFr);
    bufSend[lenFr++] = cks;
    bufSend[lenFr++] = MSG_TEMINATE;
    UART_sendFrame((uint8_t*)bufSend, lenFr);

}

void uartVoice_sendFrameWithMsgIdx(uint8_t msgIdx, uint8_t* data, uint16_t len)
{
    uartVoice_sendFrame(msgCodeList[msgIdx][0], data, len);
    uartVoice_setMsgIdxIsControlling(msgIdx, false);
}

int uartVoice_getMsgIdx(uint8_t msgCode)
{
    uint8_t i = 0;
    for(i = 0; i < MSG_IDX_MAX; i++)
    {
        if(msgCodeList[i][0] == msgCode)
        {
            return i;
        }
    }
    return -1;
}

int uartVoice_getMsgCode(uint8_t msgIdx)
{
    if(msgIdx < MSG_IDX_MAX)
        return msgCodeList[msgIdx][0];
    else return -1;
}

void uartVoice_setMsgIdxIsControlling(uint8_t msgIdx, bool isControlling)
{
    uint8_t i = 0;
    int msgCode = uartVoice_getMsgCode(msgIdx);
    if(msgCode == -1) return;
    for(i = 0; i < NUM_MSG_UP_CP; i++)
    {
        if(msgIsControlling[i][0] == msgCode)
        {
            msgIsControlling[i][1] = isControlling;
        }
    }
}

bool uartVoice_msgIdxIsControlling(uint8_t msgIdx)
{
    uint8_t i = 0;
    int msgCode = uartVoice_getMsgCode(msgIdx);
    if(msgCode == -1) return false;
    for(i = 0; i < NUM_MSG_UP_CP; i++)
    {
        if(msgIsControlling[i][0] == msgCode)
        {
            return (bool)msgIsControlling[i][1];
        }
    }
    return false;
}

extern uint8_t stateWaterCurrent;
extern uint32_t s_recentFilterReset;
extern bool water_isOuting; //hot block
extern bool isVoiceCommand;
bool needUpdateGetwater = false;
bool needUpAll = false;
void uartVoice_processCommandData(uint8_t* command, uint16_t length)
{
    if(command[0] == MSG_HEADER && command[length - 1] == MSG_TEMINATE)
    {
        uint8_t lenData = command[2];
        uint8_t* pDataStart = &command[3];
        uint8_t cks = uartVoice_calculateChecksum(command, length - 2);  //k tinh byte terminate va cks
        if(cks == command[length - 2])
        {
            int msgIdx = uartVoice_getMsgIdx(command[1]);
            switch(command[1])
            {
				//command[3]: data start
				case MSG_CODE_REQUEST_GET_DATA:
					if(*pDataStart == 0)
					{
						//get all
						updateParam_updateIo();
						needUpdateGetwater = true;
                        needUpAll = true;
					}
					else
					{
                        int idx = uartVoice_getMsgIdx(*pDataStart);
						if(idx != -1)
						{
							  //get exactly command
                            if(idx == MSG_IDX_UP_VAN_XA) updateParam_setNeedUpdate(PARAM_TYPE_VX);
                            else if(idx == MSG_IDX_UP_BOM) updateParam_setNeedUpdate(PARAM_TYPE_BOM);
                            else if(idx == MSG_IDX_UP_BINH_AP) updateParam_setNeedUpdate(PARAM_TYPE_AC);
                            else if(idx == MSG_IDX_UP_NUOC_VAO) updateParam_setNeedUpdate(PARAM_TYPE_AT);
                            else if(idx == MSG_IDX_UP_HSD_REMAIN) updateParam_setNeedUpdate(PARAM_TYPE_TIMEEXPIRE);
                            else if(idx == MSG_IDX_UP_HSD_DEFAULT) updateParam_setNeedUpdate(PARAM_TYPE_EXPIRETOTAL);
                            else if(idx == MSG_IDX_UP_RUNTIME_DEFAULT) updateParam_setNeedUpdate(PARAM_TYPE_FILTERTOTAL);
                            else if(idx == MSG_IDX_UP_RUNTIME_REMAIN) updateParam_setNeedUpdate(PARAM_TYPE_TIMEFILTER);
                            else if(idx == MSG_IDX_UP_TDS) updateParam_setNeedUpdate(PARAM_TYPE_TDS);
                            else if(idx == MSG_IDX_UP_ERROR) updateParam_setNeedUpdate(PARAM_TYPE_ERRS);
                            else if(idx == MSG_IDX_UP_TEMP_HOT) updateParam_setNeedUpdate(PARAM_TYPE_TEMPHOT);
                            else if(idx == MSG_IDX_UP_TEMP_COLD) updateParam_setNeedUpdate(PARAM_TYPE_TEMPCOLD);
                            else if(idx == MSG_IDX_UP_CP_RESET_FITLER) updateParam_setNeedUpdate(PARAM_TYPE_RSFT);
                            else if(idx == MSG_IDX_UP_CP_ENABLE_COLDTANK) updateParam_setNeedUpdate(PARAM_TYPE_CTRLCOLD);
                            else if(idx == MSG_IDX_UP_CP_ENABLE_HOTTANK) updateParam_setNeedUpdate(PARAM_TYPE_CTRLHOT);
                            else if(idx == MSG_IDX_UP_CP_SETTING_FASTBLOCK) updateParam_setNeedUpdate(PARAM_TYPE_CTRLDISP);
                            else if(idx == MSG_IDX_UP_CP_GET_WATER){
                                uint8_t dataGetWater[3];
                                dataGetWater[0] = 0; dataGetWater[1] = 0; dataGetWater[2] = stateWaterCurrent;
                                uartVoice_sendFrame(MSG_CODE_UP_CP_GET_WATER, dataGetWater, 3);
                            }

						}
					}
					break;
				case MSG_CODE_UP_CP_ENABLE_COLDTANK:
                    {
                    	uartVoice_setMsgIdxIsControlling(msgIdx, true);
                    	thermo_controlEnableTankCold(*(pDataStart+1));
                    }
					break;
				case MSG_CODE_UP_CP_ENABLE_HOTTANK:
                    uartVoice_setMsgIdxIsControlling(msgIdx, true);
                    thermo_controlEnableTankHot(*(pDataStart+1));
					break;
				case MSG_CODE_UP_CP_SETTING_FASTBLOCK:
                    uartVoice_setMsgIdxIsControlling(msgIdx, true);
                    heatControl_setSettingDefaultIdx(*(pDataStart+1), *(pDataStart+2));
					break;
				case MSG_CODE_UP_CP_RESET_FITLER:
					if(command[3] < FILTER_NUM)
					{
                        s_recentFilterReset = *(pDataStart+1);
                        uartVoice_setMsgIdxIsControlling(msgIdx, true);
                        AppConfig_resetTimeIndex((uint8_t)s_recentFilterReset);
                        if(s_recentFilterReset == 4)
                        {
                             //loi C
//                            gpio_setNeedXaSucWhenResetFilterC(); //->update UI
                        }
					}
					break;
				case MSG_CODE_UP_CP_GET_WATER:
				{
                    if(gpio_isXaSuc() && (gpio_xaSucType() != 5))
                    {
                        break;
                    }
                    if(gpio_xaSucType() == 5)
                    {
                        gpio_setResumePauseXaSucAfterGetWater(false, false);
                    }
                    isVoiceCommand = *(pDataStart+1);
                    if(*(pDataStart+2) == 1)
					{
                        getWater_controlGetWater(WATER_TYPE_RO, true);
					}
                    else if(*(pDataStart+2) == 2)
					{
						//hot tam thoi do mcu voice dieu khien, chi nhan biet onoff de log
                        // TURN_ON_VDT_4;
						water_isOuting = true;
					}
                    else if(*(pDataStart+2) == 3)
					{
						getWater_controlGetWater(WATER_TYPE_COOL, true);
					}
                    else if(*(pDataStart+2) == 4)
					{
						getWater_controlGetWater(WATER_TYPE_COLD, true);
					}
                    else if(*(pDataStart+2) == 5)
                    {
//                       getWater_controlGetWater(WATER_TYPE_H2CAP, true);
					}
					else if(*(pDataStart+2) == 6)
					{
					   getWater_controlGetWater(WATER_TYPE_PH85, true);
					}
					else if(*(pDataStart+2) == 7)
					{
					   getWater_controlGetWater(WATER_TYPE_PH90, true);
					}
					else if(*(pDataStart+2) == 8)
					{
					   getWater_controlGetWater(WATER_TYPE_PH95, true);
                    }
					else{
						getWater_controlTurnOffAllWater();
						water_isOuting = false;
                        getWater_needDpAfterStopWater();
					}

//                    uartVoice_setMsgIdxIsControlling(msgIdx, true);
					uartVoice_sendFrame(MSG_CODE_UP_CP_GET_WATER, pDataStart, 3);
//                    uartVoice_setMsgIdxIsControlling(msgIdx, false);
					break;
				}
                case MSG_CODE_UP_CP_XA_SUC:
                {
                    if(*(pDataStart+1) == 0)
                    {
//                        gpio_resetAllXaSuc();
                        if(gpio_xaSucType() == 5)
                        {
                            gpio_setResumePauseXaSucAfterGetWater(false, true);
                        }
                        else if(gpio_xaSucType() == 2)
                        {
                        	gpio_resetAllXaSuc();
                        	// TURN_OFF_VDT_4;
                        	// TURN_OFF_VDT_6;
                        	// TURN_OFF_VDT_3;
                        	// TURN_OFF_VDT_7;
                            TURN_OFF_VDT_2;
                            TURN_OFF_VDT_6;
                        }
                        else if(gpio_xaSucType() == 6)
                        {
                            gpio_resetAllXaSuc();
                            TURN_OFF_VDT_5;
                        }
                    }
                    else if(*(pDataStart+1) == 3) //xa suc reset C
                    {
                        gpio_setNeedXaSucWhenResetFilterC();
                    }
                    else if(*(pDataStart+1) == 6) //xa suc loi
                    {
                        gpio_setNeedXaSucAfterResetFilter();
                    }
                    uartVoice_sendFrame(MSG_CODE_UP_CP_XA_SUC, pDataStart, 2);
                    break;
                }
				default:
					break;

            }
        }
    }
}

//nhieu frame trong 1 buffer
void uartVoice_processFrameFormat(uint8_t *buffer_rx, uint16_t length)
{
    uint16_t idx = 0;
    if(length < 5) return;  //hien chua co lenh nao <4 byte
    while(idx < length - 2)
    {
        if(buffer_rx[idx] == MSG_HEADER)
        {
            int msgIdx = uartVoice_getMsgIdx(buffer_rx[idx+1]);
            if(msgIdx != -1)
            {
                uint16_t lengthExpect = msgCodeList[msgIdx][1];       //len expect
                if(lengthExpect == buffer_rx[idx+2])
                {
                    if(length < (idx+5 + lengthExpect)) return;  //+1header+1msgcode+1len(n)+1cks+1terminate

                    uartVoice_processCommandData(&buffer_rx[idx], lengthExpect+5);
                    idx = idx+5+lengthExpect;
                }
                else
                {
                    idx++;
                }
            }
            else
            {
                idx++;
            }

        }
        else {
            idx++;
        }
    }
}

void uartVoice_sendResetToMcu()
{
    uint8_t dataReset[1] = {0};
    uartVoice_sendFrameWithMsgIdx(MSG_IDX_REQUEST_GET_DATA, (uint8_t*)dataReset, 1);

}



/*
 * tools.c
 *
 *  Created on: Aug 17, 2018
 *      Author: Admin
 */

#include "tools.h"
#include "timeCheck.h"


int mySoftwareReset(void) {

   SYSTEM.PRCR.WORD = 0xA502;  /* Enable writing to the Software Reset */

   SYSTEM.SWRR = 0xA501;            /* Software Reset */

   SYSTEM.PRCR.WORD = 0xA500;  /* Disable writing to the Software Reset */

   return 0;

}
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (x - in_min)*(out_max - out_min)/(in_max - in_min)+out_min;
}
void tools_softDelay(uint32_t ms)
{
	uint32_t lastTime = g_sysTime;
	while(elapsedTime(g_sysTime, lastTime)<ms);
}


uint8_t tools_decodeCheckSum(uint8_t high, uint8_t low){
	if (high > 0x40)
		high = high - 0x37;
	else
		high = high - 0x30;
	if (low > 0x40)
		low = low - 0x37;
	else
		low = low - 0x30;
	return ((high << 4) + low); // ket qua tra ve khac voi return (high <<4+low)
}
void tools_encodeCheckSum(uint8_t checksum, char *high, char *low){

	*high = (checksum & 0xF0) >> 4;
	*low = checksum & 0x0F;
	if ( *high < 0x0A)
		*high = *high + 0x30;
	else
		*high = *high + 0x37;
	if ( *low < 0x0A)
		*low = *low + 0x30;
	else
		*low = *low + 0x37;

}
uint8_t tools_calculateChecksum(uint8_t *buf, uint32_t len) {
	uint8_t chk = 0;
	uint32_t i = 0;
	for (i = 0; i < len; i++) {
		chk ^= buf[i];
	}
	return chk;
}

uint16_t tools_calCheckSum2Byte(uint8_t *buf, uint32_t len)
{
	uint16_t chk = 0;
	uint32_t i = 0;
	for (i = 0; i < len; i++) {
		chk += buf[i];
	}
	return chk;
}

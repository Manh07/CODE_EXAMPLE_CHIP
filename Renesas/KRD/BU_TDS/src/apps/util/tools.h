/*
 * tools.h
 *
 *  Created on: Aug 17, 2018
 *      Author: Admin
 */

#ifndef APPS_UTIL_TOOLS_H_
#define APPS_UTIL_TOOLS_H_
#include "config.h"

int mySoftwareReset(void);
void tools_softDelay(uint32_t ms);
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
uint8_t tools_calculateChecksum(uint8_t *buf, uint32_t len);
void tools_encodeCheckSum(uint8_t checksum, char *high, char *low);
uint8_t tools_decodeCheckSum(uint8_t high, uint8_t low);
uint16_t tools_calCheckSum2Byte(uint8_t *buf, uint32_t len);
#endif /* APPS_UTIL_TOOLS_H_ */

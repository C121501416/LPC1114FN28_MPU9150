/*
 * Project:		LPC1114_PwmConverter
 * File:		xa_s.c
 * Author:		Terukazu Sato
 * Version:		2.0.0
 *
 */

#include "xa_s.h"

#if XA_S_SYSVER != 0x0200
  #error "xa_s.h version mismatch!"
#else

int XAS_In(uint8_t qty_byte, uint8_t *rBuf)
{
	int i, temp, checksum = 0, loop = qty_byte>>4;

	for (i=0; i<loop; i++) {
		temp = RS485_RecvData();
		if (temp == NO_RESPONSE) return NO_RESPONSE;
		else rBuf[i] = (uint8_t)temp;
	}

	for (i=0; i<loop-1; i++) {
		checksum += rBuf[i];
	}
	if (rBuf[i] != (checksum&0xff)) return ERR_CHECKSUM;
	return 0;
}

int XAS_Out(uint8_t qty_byte, uint8_t *tBuf)
{
	int i, loop = qty_byte&0x0f;

	for (i=0; i<loop; i++) {
		RS485_SendData(tBuf[i]);
	}
	return 0;
}

void XAS_OutERR(uint8_t qty_byte, uint8_t errData)
{
	int i;
	for (i=0; i<(qty_byte&0x0f); i++) {
		RS485_SendData(errData);
	}
}

#endif

/*
 * **************************************************************************
 * 	@file		spi.h
 * 	@author		T.Sato
 * 	@version	1.00
 * 	@date		2013/1/11
 * **************************************************************************
 */

#ifndef __SPI_H__
#define __SPI_H__

void SPI_Init(int freq);
uint16_t SPI_Xmit(uint16_t sendData);

#endif /* __SPI_H__ */

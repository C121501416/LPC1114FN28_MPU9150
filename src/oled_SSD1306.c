/*
 * @file	oled_SSD1306.c
 * @author	teru
 * @date	2013/12/7
 *
 */


/* Includes ------------------------------------------------------- */
#include "lpc_types.h"
#include "lpc11xx_gpio.h"
#include "lpc11xx_ssp.h"
#include "oled_SSD1306.h"
#include "tri_S_Font6x8_90.h"
#include "systick.h"

/* Defines -------------------------------------------------------- */
/* Variables ------------------------------------------------------ */
static int charpos_x,charpos_y;


/* @brief	Write command to OLED
 * @param	Command(1byte)
 * @retval	None
 */
void OLED_WriteCommand(uint8_t cmd)
{
	OLED_CS_L();
	OLED_AO_L();
	while(!(OLED_SSP_PORT->SR & SSP_STAT_TXFIFO_NOTFULL));
	OLED_SSP_PORT->DR = cmd;
	OLED_CS_H();
}


/* @brief	Write data to OLED
 * @param	Data(1byte)
 * @retval	None
 */
void OLED_WriteData(uint8_t data)
{
	OLED_CS_L();
	OLED_AO_H();
	while(!(OLED_SSP_PORT->SR & SSP_STAT_TXFIFO_NOTFULL));
	OLED_SSP_PORT->DR = data;
	OLED_CS_H();
}


/* @brief	OLED initialization
 * @param	None
 * @retval	None
 */
void OLED_Init()
{
	/* OLED Reset */
	OLED_RST_H();
	_delay_ms(50);
	OLED_RST_L();
	_delay_ms(50);
	OLED_RST_H();
	_delay_ms(50);
	/* ---------- */

	OLED_WriteCommand(0xAE);     //Set Display Off
	OLED_WriteCommand(0xd5);     //display divide ratio/osc. freq. mode
	OLED_WriteCommand(0x80);     //
	OLED_WriteCommand(0xA8);     //multiplex ration mode:63
	OLED_WriteCommand(0x3F);
	OLED_WriteCommand(0xD3);     //Set Display Offset
	OLED_WriteCommand(0x00);
	OLED_WriteCommand(0x40);     //Set Display Start Line
	OLED_WriteCommand(0x8D);     //Set Display Offset
	//OLED_WriteCommand(0x10);
	OLED_WriteCommand(0x14);
	OLED_WriteCommand(0xA1);     //Segment Remap
	OLED_WriteCommand(0xC8);     //Sst COM Output Scan Direction
	//OLED_WriteCommand(0xC8);
	OLED_WriteCommand(0xDA);     //common pads hardware: alternative
	OLED_WriteCommand(0x12);
	OLED_WriteCommand(0x81);     //contrast control
	//OLED_WriteCommand(0x9F);     //
	OLED_WriteCommand(0x66);
	OLED_WriteCommand(0xD9);	    //set pre-charge period
	//OLED_WriteCommand(0x22);	    //set period 1:1;period 2:15
	OLED_WriteCommand(0xF1);
	OLED_WriteCommand(0xDB);     //VCOM deselect level mode
	OLED_WriteCommand(0x40);	    //set Vvcomh=0.83*Vcc
	OLED_WriteCommand(0xA4);     //Set Entire Display On/Off
	OLED_WriteCommand(0xA6);     //Set Normal Display
	OLED_WriteCommand(0xAF);     //Set Display On
}


/* @brief	All clear OLED
 * @param	None
 * @retval	None
 */
void OLED_Clear()
{
	uint8_t k,i;
	for(k=0;k<8;k++)
	{
		OLED_WriteCommand(0xb0+k);
		OLED_WriteCommand(0x10);
		OLED_WriteCommand(0x00);
		for(i=0;i<128;i++)
		{
			OLED_WriteData(0x00);
		}
	}
}


void OLED_Pixel(int x, int y)
{
	uint8_t page,column_h,column_l;
	page = y>>3;
	column_h = (x & 0xf0)>>4;
	column_l = (x & 0x0f);
	OLED_WriteCommand(0xb0+page);
	OLED_WriteCommand(0x10+column_h);
	OLED_WriteCommand(0x00+column_l);
	y = y%8;
	OLED_WriteData(1<<y);
}


void OLED_Pos(int x, int y)
{
	uint8_t page,column_h,column_l;
	page = (y>>3)&0x07;
	column_h = (x & 0xf0)>>4;
	column_l = (x & 0x0f);
	OLED_WriteCommand(0xb0+page);
	OLED_WriteCommand(0x10+column_h);
	OLED_WriteCommand(0x00+column_l);
}

void OLED_Locate(int column, int row)
{
	charpos_x = column;
	charpos_y = row;
}

void OLED_DrawChar(uint8_t AsciiCode)
{
	int data;
	data = AsciiCode*6;
	if (AsciiCode == '\n') {
		//charpos_x = 0;
		charpos_y += 1;
		return;
	}
	if (AsciiCode == '\r') {
		charpos_x = 0;
		return;
	}
	OLED_Pos(charpos_x*6,charpos_y*8);
	OLED_WriteData(tri_S_uchrFont6x8_90[data+0]);
	OLED_WriteData(tri_S_uchrFont6x8_90[data+1]);
	OLED_WriteData(tri_S_uchrFont6x8_90[data+2]);
	OLED_WriteData(tri_S_uchrFont6x8_90[data+3]);
	OLED_WriteData(tri_S_uchrFont6x8_90[data+4]);
	OLED_WriteData(tri_S_uchrFont6x8_90[data+5]);
	charpos_x++;
}
void OLED_DrawLine(int x0, int y0, int x1, int y1)
{
    short steep, t;
    short deltax, deltay, error;
    short x, y;
    short ystep;

    y0=y0 -1;
    y1=y1 -1;
    x0=x0 -1;
    x1=x1 -1;
    steep = (abs(y1 - y0) > abs(x1 - x0));
    if(steep){
            t = x0; x0 = y0; y0 = t;
            t = x1; x1 = y1; y1 = t;
    }
    if(x0 > x1) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
    }
    deltax = x1 - x0;
    deltay = abs(y1 - y0);
    error = 0;
    y = y0;
    if(y0 < y1) ystep = 1; else ystep = -1;
    for(x=x0; x<=x1; x++) {
            if(steep) {
            	OLED_Pixel(y,x);
            }
            else {
            	OLED_Pixel(x,y);
            }
            error += deltay;
            if((error << 1) >= deltax) {
                    y += ystep;
                    error -= deltax;
            }
    }
}

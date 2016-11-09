/*
 * @file	oled_SSD1306.h
 * @author	teru
 *
 */

#ifndef __OLED_SSD1306_H__
#define __OLED_SSD1306_H__

#define OLED_AO			GPIO_Pin_2
#define OLED_RST		GPIO_Pin_1
#define OLED_CS			GPIO_Pin_3
#define OLED_SSP_PORT	LPC_SSP0
#define OLED_CTRL_PORT	PORT0
#define OLED_CS_H()		GPIO_SetBits(OLED_CTRL_PORT, OLED_CS)
#define OLED_CS_L()		GPIO_ResetBits(OLED_CTRL_PORT, OLED_CS)
#define OLED_AO_H()		GPIO_SetBits(OLED_CTRL_PORT, OLED_AO)
#define OLED_AO_L()		GPIO_ResetBits(OLED_CTRL_PORT, OLED_AO)
#define OLED_RST_H()	GPIO_SetBits(OLED_CTRL_PORT, OLED_RST)
#define OLED_RST_L()	GPIO_ResetBits(OLED_CTRL_PORT, OLED_RST)

void OLED_WriteCommand(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Pixel(int x, int y);
void OLED_Pos(int x, int y);
void OLED_Locate(int column, int row);
void OLED_DrawChar(uint8_t AsciiCode);

#define abs(a)  (((a)>0) ? (a) : -(a))



#endif /* __OLED_SSD1306_H__ */
